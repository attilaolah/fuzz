#include "from_proto.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include <cstdint>
#include <cstring>

#include <gif_lib.h>

#include "src/gif/gif.pb.h"

namespace gif {
namespace {
constexpr char kGif99a[] = "GIF99a";
constexpr size_t kMaxSize = 64 * 1024 * 1024; // 64 MiB, approximate

void fill_with_zeros(std::vector<uint8_t> &data, size_t n) {
  data.insert(data.end(), std::min(kMaxSize - data.size(), n), 0);
}

void fill_with_random(std::vector<uint8_t> &data, std::mt19937 &rg, size_t n) {
  static std::uniform_int_distribution<uint8_t> uint8_d{
      std::numeric_limits<uint8_t>::min(),
      std::numeric_limits<uint8_t>::max(),
  };
  for (int i = 0; i < n && data.size() < kMaxSize; i++) {
    data.push_back(uint8_d(rg));
  }
}

void write_colours(std::vector<uint8_t> &gif, const ColourMap &cm) {
  const size_t colour_count = 1 << (((cm.bits_per_pixel() - 1) & 0b111) + 1);
  if (colour_count == 0) {
    return;
  }
  if (cm.has_values()) {
    const auto &values = cm.values();
    if (values.rgb().empty()) {
      // Nothing to write, just return.
      return;
    }
    for (size_t i = 0; i < colour_count && gif.size() < kMaxSize; i++) {
      const RGB &rgb = values.rgb(i % values.rgb().size());
      gif.push_back(rgb.r() & 0xff);
      gif.push_back(rgb.g() & 0xff);
      gif.push_back(rgb.b() & 0xff);
    }
  } else if (cm.has_zeros()) {
    fill_with_zeros(gif, colour_count * 3);
  }
}

int append_to_vector(GifFileType *file, const GifByteType *data, int size) {
  auto *vec = static_cast<std::vector<uint8_t> *>(file->UserData);
  // If user-data is null, ignore the write, but report as if it was successful.
  if (vec != nullptr) {
    vec->insert(vec->end(), data, data + size);
  }
  return size;
}
} // namespace

auto from_proto(const Gif &proto) -> std::vector<uint8_t> {
  std::vector<uint8_t> gif = {};

  std::random_device rd;
  std::mt19937 rg(rd());

  // GIF header.
  switch (proto.version()) {
  case ZEROS:
    fill_with_zeros(gif, GIF_STAMP_LEN);
    break;
  case GIF87:
    gif.insert(gif.end(), GIF87_STAMP, &GIF87_STAMP[GIF_STAMP_LEN]);
    break;
  case GIF89:
    gif.insert(gif.end(), GIF89_STAMP, &GIF89_STAMP[GIF_STAMP_LEN]);
    break;
  case GIF99:
    gif.insert(gif.end(), kGif99a, &kGif99a[sizeof(kGif99a)]);
    break;
  case RANDOM_VERSION:
    fill_with_random(gif, rg, GIF_STAMP_LEN);
    break;
  default:
    break; // should never happen
  }

  // Height & width.
  uint32_t w = proto.width() & 0xffff;
  uint32_t h = proto.height() & 0xffff;
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

  uint8_t bits_per_pixel = 0;

  // Global colour map.
  auto cm = proto.colour_map();
  uint8_t byte = ((proto.colour_resolution() - 1) & 0b111) << 4; // 01110000
  if (proto.has_colour_map()) {
    byte |= 0x80;                              // 10000000
    byte |= (cm.is_sorted() ? 1 : 0) << 3;     // 00001000
    byte |= (cm.bits_per_pixel() - 1) & 0b111; // 00000111
    gif.push_back(byte);

    bits_per_pixel = cm.bits_per_pixel() & 0xff;
  }

  // Background colour might be out of range for pictures with < 8-bit colours.
  gif.push_back(proto.background_colour() & 0xff);

  // GIF_ASPECT_RATIO turns this into n+15.0/64.0, so:
  gif.push_back(proto.aspect_ratio() & 0xff);

  if (proto.has_colour_map()) {
    write_colours(gif, proto.colour_map());
  }

  for (const auto &block : proto.blocks()) {
    if (block.has_ext()) {
      // EXTENSION_INTRODUCER
      gif.push_back(0x21);

      const auto &ext = block.ext();
      const std::string &content = ext.content();

      // Extension (function) code:
      switch (ext.code()) {
      case CONTINUATION:
        gif.push_back(0x00);
      case PLAIN_TEXT:
        gif.push_back(0x01);
        break;
      case COMMENT:
        gif.push_back(0xfe);
        break;
      case GRAPHICS:
        gif.push_back(0xf9);
        break;
      case APPLICATION:
        gif.push_back(0xff);
        break;
      case RANDOM_FUNCTION:
        fill_with_random(gif, rg, 1);
      default:
        break; // should never happen
      }

      // Extension block content:
      // - 1 byte extension length
      // - 0-255 byte extension data
      if (content.empty()) {
        gif.push_back(0x00);
        continue;
      }
      for (size_t i = 0; i < content.size() && gif.size() < kMaxSize; i++) {
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

      if (gif.size() >= kMaxSize) {
        return gif;
      }

      // NOTE: Extension blocks are only applied to the next image that is read.
      continue;
    }

    if (block.has_desc()) {
      // DESCRIPTOR_INTRODUCER
      gif.push_back(0x2c);

      const auto &desc = block.desc();

      // Image position & dimensions.
      uint32_t l = desc.left() & 0xffff;
      uint32_t t = desc.top() & 0xffff;
      uint32_t w = desc.width() & 0xffff;
      uint32_t h = desc.height() & 0xffff;
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

      uint8_t img_bits_per_pixel = bits_per_pixel;
      uint8_t byte = desc.interlaced() ? 0x80 : 0x00; // 10000000
      if (desc.has_colour_map()) {
        const auto &cm = desc.colour_map();
        byte |= 0b1000;                      // 00001000
        byte |= cm.bits_per_pixel() & 0b111; // 00000111
        gif.push_back(byte);
        write_colours(gif, cm);
        img_bits_per_pixel = cm.bits_per_pixel() & 0xff;
      } else {
        gif.push_back(byte);
      }

      // Image code size, which is really just the bits-per-pixel.
      gif.push_back(img_bits_per_pixel);

      if (h * w == 0) {
        // Not really a valid image, but oh well.
        continue;
      }

      // Write (height * width) bytes of pixel data.
      if (desc.has_values() && !desc.values().empty()) {
        if (!(proto.has_colour_map() || desc.has_colour_map())) {
          // At least one colour map (global or image-local) is necessary.
          // If none exist, we can't write any image data.
          continue;
        }

        // Create a new GIF file using the giflib library.
        // We will discard the results of anything except the image data.
        GifFileType *f = EGifOpen(nullptr, append_to_vector, nullptr);
        if (f == nullptr) {
          throw std::runtime_error("GIF file could not be created");
        }

        // Set the image dimensions.
        if (EGifPutScreenDesc(f, w, h, proto.colour_resolution(),
                              proto.background_colour() & 0xff,
                              nullptr) != GIF_OK) {
          throw std::runtime_error(GifErrorString(f->Error));
        }

        // Colour map.
        const auto &cm =
            proto.has_colour_map() ? proto.colour_map() : desc.colour_map();
        const size_t colour_count =
            1 << (((cm.bits_per_pixel() - 1) & 0b111) + 1);
        if (colour_count == 0) {
          // We need at least some colours for writing image data.
          continue;
        }
        ColorMapObject *cmap = GifMakeMapObject(colour_count, nullptr);
        /*
         * NOTE: It would be possible to copy colour values too, if needed:
        if (cm.has_values()) {
          const auto &values = cm.values();
          if (values.rgb().empty()) {
            continue;
          }
          for (size_t i = 0; i < colour_count; i++) {
            const RGB &rgb = values.rgb(i % values.rgb().size());
            cmap->Colors[i].Red = rgb.r() & 0xff;
            cmap->Colors[i].Green = rgb.g() & 0xff;
            cmap->Colors[i].Blue = rgb.b() & 0xff;
          }
        } else if (cm.has_zeros()) {
          for (size_t i = 0; i < colour_count; i++) {
            cmap->Colors[i].Red = 0x00;
            cmap->Colors[i].Green = 0x00;
            cmap->Colors[i].Blue = 0x00;
          }
        }*/

        // Write the image description.
        if (EGifPutImageDesc(f, l, t, w, h, desc.interlaced(), cmap) !=
            GIF_OK) {
          throw std::runtime_error(GifErrorString(f->Error));
        }

        // Enable writing.
        f->UserData = &gif;

        // TODO: This won't fly with interlaced images!
        const int line_len = w * sizeof(GifByteType);
        for (int y = 0; y < h; y++) {
          const uint8_t *line = reinterpret_cast<const uint8_t *>(
              desc.values().data() + (line_len * y));
          if (EGifPutLine(f, const_cast<GifPixelType *>(line), line_len) !=
              GIF_OK) {
            throw std::runtime_error(GifErrorString(f->Error));
          }
        }

        // Disable writing.
        f->UserData = nullptr;

        // Close the GIF file
        EGifCloseFile(f, nullptr);
      } else if (desc.has_zeros()) {
        fill_with_zeros(gif, h * w);
      }

      if (gif.size() >= kMaxSize) {
        return gif;
      }

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
} // namespace gif
