#pragma once

#include <cstdint>
#include <vector>

#include "src/gif/gif.pb.h"

namespace gif {
auto from_proto(const Gif &proto) -> std::vector<uint8_t>;
} // namespace gif
