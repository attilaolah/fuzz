#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <gif_lib.h>

#include "src/gif/span.h"

namespace gif {
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  return 0;

  Span span{.data = data, .size = size};
  int err;

  if (GifFileType *gif = DGifOpen(&span, &Span::read, &err); gif != nullptr) {
    DGifSlurp(gif);
    DGifCloseFile(gif, &err);
  }
}

} // namespace
