#pragma once

#include <cstdint>

#include <gif_lib.h>

namespace utils {
// NOTE: We should use std::span, but
// getting it to compile with all the deps is painful.
struct Span {
  const uint8_t *data;
  size_t size;
};

auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int;
} // namespace utils
