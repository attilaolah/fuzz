#include "utils.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

#include <gif_lib.h>

#include "lib/gif/fuzz.pb.h"

namespace utils {
using ::fuzz::Gif;

auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int {
  auto src = static_cast<Span *>(gif->UserData);
  if (src == nullptr) {
    return 0;
  }

  size = std::min(static_cast<int>(src->size), size);
  std::memcpy(dst, src->data, size);
  src->data += size;
  src->size -= size;
  return size;
}

auto proto_to_gif(const Gif &proto) -> std::vector<uint8_t> {
  std::vector<uint8_t> gif = {};

  // GIF header.
  switch (proto.version()) {
  case Gif::GIF87:
    gif.insert(gif.end(), GIF87_STAMP, &GIF87_STAMP[GIF_STAMP_LEN]);
    break;
  case Gif::GIF89:
    gif.insert(gif.end(), GIF89_STAMP, &GIF89_STAMP[GIF_STAMP_LEN]);
    break;
  case Gif::UNKNOWN:
  default:
    // No header. This will be an invalid image.
    // TODO: Maybe enforce a valid image no matter what?
    break;
  }

  // Height & width.
  uint16_t w = proto.width() & 0xffff;
  uint16_t h = proto.height() & 0xffff;
  while (w * h > 0 && w > (std::numeric_limits<int32_t>::max() / h)) {
    if (w > h) {
      w /= 2;
    } else {
      h /= 2;
    }
  }
  gif.push_back(w & 0xff);
  gif.push_back(w >> 8);
  gif.push_back(h & 0xff);
  gif.push_back(h >> 8);

  // Colour map.
  auto cm = proto.colour_map();
  if (proto.has_colour_map()) {
    uint8_t byte = 0x80;                           // 10000000
    byte |= (cm.colour_resolution() & 0b111) << 4; // 01110000
    byte |= (cm.is_sorted() ? 1 : 0) << 3;         // 00001000
    byte |= cm.bits_per_pixel() & 0b111;           // 00000111
    gif.push_back(byte);
  }

  // Background colour might be out of range for pictures with < 8-bit colours.
  gif.push_back(proto.background_colour() & 0xff);

  // GIF_ASPECT_RATIO turns this into n+15.0/64.0, so:
  gif.push_back(proto.aspect_ratio() & 0xff);

  if (proto.has_colour_map()) {
    uint8_t colour_count = 1 << ((cm.bits_per_pixel() & 0b111) + 1);
    for (size_t i = 0; i < colour_count; i++) {
      // TODO: Maybe provide a colour palette, e.g. via a bytes field?
      gif.push_back(0x00); // R
      gif.push_back(0x00); // G
      gif.push_back(0x00); // B
    }
  }

  for (const auto &block : proto.blocks()) {
    if (block.has_ext()) {
      // EXTENSION_INTRODUCER
      gif.push_back(0x21);

      const auto &ext = block.ext();
      const std::string &content = ext.content();

      // Extension (function) code:
      using Ext = Gif::Block::Extension;
      switch (ext.code()) {
      case Ext::PLAIN_TEXT:
        gif.push_back(0x01);
        break;
      case Ext::COMMENT:
        gif.push_back(0xfe);
        break;
      case Ext::GRAPHICS:
        gif.push_back(0xf9);
        break;
      case Ext::APPLICATION:
        gif.push_back(0xff);
        break;
      case Ext::UNKNOWN:
      default:
        gif.push_back(0x00);
        break;
      }

      // Extension block content:
      // - 1 byte extension length
      // - 0-255 byte extension data
      if (content.empty()) {
        gif.push_back(0x00);
        continue;
      }
      for (size_t i = 0; i < content.size(); i++) {
        if ((i & 0xff) == 0) {
          if (i != 0) {
            // This is not the first chunk of 255 bytes, i.e. a continuation.
            // Write a CONTINUE_EXT_FUNC_CODE marker to signal the next block.
            gif.push_back(0x00);
          }
          // Before writing the first byte, write how many bytes there are.
          gif.push_back(std::min(content.size() - i, 0xffUL) & 0xff);
        }
        gif.push_back(content[i]);
      }

      // NOTE: Extension blocks are only applied to the next image that is read.
      continue;
    }

    if (block.has_desc()) {
      // DESCRIPTOR_INTRODUCER
      gif.push_back(0x2c);

      const auto &desc = block.desc();

      // Image position & dimensions.
      uint16_t l = desc.left() & 0xffff;
      uint16_t t = desc.top() & 0xffff;
      uint16_t w = desc.width() & 0xffff;
      uint16_t h = desc.height() & 0xffff;
      while (w * h > 0 && w > (std::numeric_limits<int32_t>::max() / h)) {
        if (w > h) {
          w /= 2;
        } else {
          h /= 2;
        }
      }
      gif.push_back(l & 0xff);
      gif.push_back(l >> 8);
      gif.push_back(t & 0xff);
      gif.push_back(t >> 8);
      gif.push_back(w & 0xff);
      gif.push_back(w >> 8);
      gif.push_back(h & 0xff);
      gif.push_back(h >> 8);

      uint8_t byte = desc.interlaced() ? 0x80 : 0x00; // 10000000
      if (desc.has_colour_map()) {
        const auto &cm = desc.colour_map();
        byte |= 0b1000;                      // 00001000
        byte |= cm.bits_per_pixel() & 0b111; // 00000111
        gif.push_back(byte);

        uint8_t colour_count = 1 << ((cm.bits_per_pixel() & 0b111) + 1);
        for (size_t i = 0; i < colour_count; i++) {
          // TODO: Maybe provide a colour palette, e.g. via a bytes field?
          gif.push_back(0x00); // R
          gif.push_back(0x00); // G
          gif.push_back(0x00); // B
        }
      } else {
        gif.push_back(byte);
      }

      // A valid code size.
      gif.push_back((desc.code_size() % 7) + 1);

      // TODO: Write (height * width) bytes of pixel data.
      // This needs to be LZ-encoded for a valid image, maybe parse some existing data.
      continue;
    }

    if (block.has_terminator()) {
      // TERMINATOR_INTRODUCER
      gif.push_back(0x3b);
      break;
    }
  }

  return gif;
}
} // namespace utils
