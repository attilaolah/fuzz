#include <algorithm>
#include <memory>
#include <span>

#include <cstdint>

#include <gif_lib.h>
#include <libprotobuf-mutator/src/libfuzzer/libfuzzer_macro.h>

#include "src/gif/from_proto.h"
#include "src/gif/gif.pb.h"
#include "src/gif/test_util.h"

namespace gif {

DEFINE_PROTO_FUZZER(const Gif &proto) {
  const auto data = from_proto(proto);
  auto span = std::make_unique<std::span<const uint8_t>>(data);
  int err;

  if (GifFileType *gif = DGifOpen(&span, read_span, &err); gif != nullptr) {
    DGifSlurp(gif);
    DGifCloseFile(gif, &err);
  }
}

} // namespace gif
