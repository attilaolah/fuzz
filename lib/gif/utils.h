#pragma once

#include <cstdint>
#include <vector>

#include <gif_lib.h>

#include "lib/gif/fuzz.pb.h"

namespace utils {
// NOTE: We should use std::span, but
// getting it to compile with all the deps is painful.
struct Span {
  const uint8_t *data;
  size_t size;
};

auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int;

auto proto_to_gif(const ::fuzz::Gif &proto) -> std::vector<uint8_t>;
} // namespace utils
