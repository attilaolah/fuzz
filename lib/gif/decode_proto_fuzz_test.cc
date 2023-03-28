#include <libprotobuf-mutator/src/libfuzzer/libfuzzer_macro.h>

#include "lib/gif/fuzz.pb.h"
#include "utils.h"

namespace {
using ::fuzz::Gif;
using ::utils::proto_to_gif;
using ::utils::read_span;
using ::utils::Span;

DEFINE_PROTO_FUZZER(const Gif &proto) {
  const auto data = proto_to_gif(proto);
  Span span{.data = data.data(), .size = data.size()};
  int err;

  if (GifFileType *gif = DGifOpen(&span, read_span, &err); gif != nullptr) {
    DGifSlurp(gif);
    DGifCloseFile(gif, &err);
  }
}

} // namespace
