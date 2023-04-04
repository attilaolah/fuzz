#pragma once

#include <string>
#include <vector>

namespace gif {
auto read_file(const std::string &filename) -> std::vector<uint8_t>;
} // namespace gif
