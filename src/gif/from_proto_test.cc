#include <algorithm>

#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>

#include "src/gif/from_proto.h"
#include "src/gif/gif.pb.h"
#include "src/gif/test_util.h"

namespace gif {
using ::google::protobuf::TextFormat;

TEST(FromProtoTest, ReEncode) {
  auto gif = read_file("src/gif/testdata/64x64.gif");
  auto textpb = read_file("src/gif/testdata/64x64.gif.textpb");

  Gif pb;
  EXPECT_TRUE(TextFormat::ParseFromString(
      std::string(textpb.begin(), textpb.end()), &pb));
  const auto new_gif = from_proto(pb);

  // Do a proper comparison:
  EXPECT_EQ(new_gif, gif);
  // Do one more in a loop where we can print the index:
  for (size_t i = 0; i < std::min(new_gif.size(), gif.size()); i++) {
    ASSERT_EQ(new_gif[i], gif[i]) << "At byte index: " << i;
  }
}

} // namespace gif
