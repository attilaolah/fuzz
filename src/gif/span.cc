#include "span.h"

#include <algorithm>
#include <memory>
#include <span>

#include <cstring>

#include <gif_lib.h>

namespace gif {
auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int {
  auto *src = static_cast<std::unique_ptr<std::span<uint8_t>> *>(gif->UserData);
  if (src == nullptr) {
    return 0;
  }

  size = std::min(static_cast<int>((*src)->size()), size);
  std::memcpy(dst, (*src)->data(), size);
  src->reset(new std::span<uint8_t>((*src)->subspan(size)));
  return size;
}
} // namespace gif
