#include "span.h"

#include <algorithm>
#include <cstring>

#include <gif_lib.h>

namespace gif {

auto Span::read(GifFileType *gif, GifByteType *dst, int size) -> int {
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
} // namespace gif
