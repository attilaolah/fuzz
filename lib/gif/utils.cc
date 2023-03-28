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
    break; // No header. This will be an invalid image.
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
      // TODO: Maybe provide a colour palette?
      gif.push_back(0x00); // R
      gif.push_back(0x00); // G
      gif.push_back(0x00); // B
    }
  }

  for (const auto &block : proto.blocks()) {
    if (block.has_ext()) {
      // EXTENSION_INTRODUCER
      gif.push_back(0x21);

      // Extension blocks:
      // - 1 byte extension code ("extension function")
      // - 1 byte extension length
      // - 0-255 byte extension data
      //
      // Extension function codes:
      // - CONTINUE_EXT_FUNC_CODE    (0x00)
      // - COMMENT_EXT_FUNC_CODE     (0xfe)
      // - GRAPHICS_EXT_FUNC_CODE    (0xf9)
      // - PLAINTEXT_EXT_FUNC_CODE   (0x01)
      // - APPLICATION_EXT_FUNC_CODE (0xff)
      // - Other extension types may be ignored?
      //
      // The "continuation" code seems to indicate that the next extension block
      // is a continuation of the previous one. This should allow us to encode
      // arbitrarily long data in an extensino block, using continuations.
      //
      // Extension blocks are only applied to the next image that is read.
      continue;
    }

    if (block.has_desc()) {
      // DESCRIPTOR_INTRODUCER
      gif.push_back(0x2c);

      // Image descriptor blocks:
      // [left, top, width, height] = 2 bytes each (unsigned).
      // Max width * height should not exceed uintptr UINTPTR_MAX.
      // - 1 byte:
      // - 10000000 -> interlaced
      // - 00001000 -> local colour map exists
      // - 00000111 -> bits per pixel (see above for an explanation)
      //
      // Next, write the local colour map.
      // Based on the bits-per-pixel value above, write 3 bytes for each colour.
      //
      // 1 byte: code size, [0-8] inclusive
      // Then (height * width) bytes of pixel data.
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
