#include <algorithm>
#include <memory>
#include <span>

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <gif_lib.h>

#include "src/gif/test_util.h"

namespace gif {
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  auto span = std::make_unique<std::span<const uint8_t>>(data, size);
  int err;

  if (GifFileType *gif = DGifOpen(&span, read_span, &err); gif != nullptr) {
    DGifSlurp(gif);
    DGifCloseFile(gif, &err);
  }

  return 0;
}

} // namespace gif
