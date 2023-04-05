#pragma once

#include <span>

#include <cstdint>

#include <gif_lib.h>

namespace gif {
auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int;
} // namespace gif
