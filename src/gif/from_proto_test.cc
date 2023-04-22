#include <algorithm>
#include <fstream>
#include <istream>
#include <vector>

#include <cstdlib>

#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>

#include "src/gif/from_proto.h"
#include "src/gif/gif.pb.h"
#include "src/gif/test_util.h"

namespace gif {
using ::google::protobuf::TextFormat;

TEST(FromProtoTest, ReEncode) {
  const auto gif = read_file("src/gif/testdata/64x64.gif");
  const auto textpb = read_file("src/gif/testdata/64x64.gif.textpb");

  Gif pb;
  EXPECT_TRUE(TextFormat::ParseFromString(
      std::string(textpb.begin(), textpb.end()), &pb));
  const auto new_gif = from_proto(pb);

  // Do a proper comparison:
  EXPECT_EQ(new_gif, gif);
  // Do one more in a loop where we can print the index:
  for (size_t i = 0; i < std::min(new_gif.size(), gif.size()); i++) {
    EXPECT_EQ(new_gif[i], gif[i]) << "At byte index: " << i;
  }

  // To write the results into a temporary file (e.g. for inspection), pass
  // --test_env=TEST_FILE_OUT=/path/to/result.gif to the bazel test command.
  if (const char *env_var = std::getenv("TEST_FILE_OUT"); env_var != nullptr) {
    const std::string file_path(env_var);
    if (std::ofstream file(file_path, std::ios::binary); file) {
      file.write(reinterpret_cast<const char *>(new_gif.data()),
                 new_gif.size());
      file.close();
    } else {
      std::cerr << "ERROR: could not open file " << file_path << " for writing."
                << std::endl;
    }
  }
}

} // namespace gif
