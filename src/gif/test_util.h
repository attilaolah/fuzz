#pragma once

#include <span>
#include <string>
#include <vector>

#include <cstdint>

#include <gif_lib.h>

namespace gif {
auto read_file(const std::string &filename) -> std::vector<uint8_t>;
auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int;
} // namespace gif
