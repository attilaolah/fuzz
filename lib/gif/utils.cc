#include "utils.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

#include <gif_lib.h>

namespace utils {
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
} // namespace utils
