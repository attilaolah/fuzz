#pragma once

#include <cstdint>

#include <gif_lib.h>

namespace gif {
// NOTE: We should use std::span, but
// getting it to compile with all the deps is painful.
struct Span {
  const uint8_t *data;
  size_t size;

  static auto read(GifFileType *gif, GifByteType *dst, int size) -> int;
};

} // namespace gif
