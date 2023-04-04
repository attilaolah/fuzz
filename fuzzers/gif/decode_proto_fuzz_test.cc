#include <libprotobuf-mutator/src/libfuzzer/libfuzzer_macro.h>

#include "src/gif/gif.pb.h"
#include "src/gif/span.h"
#include "src/gif/from_proto.h"

namespace gif {

DEFINE_PROTO_FUZZER(const Gif &proto) {
  const auto data = from_proto(proto);
  Span span{.data = data.data(), .size = data.size()};
  int err;

  if (GifFileType *gif = DGifOpen(&span, &Span::read, &err); gif != nullptr) {
    DGifSlurp(gif);
    DGifCloseFile(gif, &err);
  }
}

} // namespace
