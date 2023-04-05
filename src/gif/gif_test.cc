#include <algorithm>
#include <span>

#include <cstdint>
#include <cstring>

#include <gif_lib.h>
#include <gtest/gtest.h>

#include "span.h"
#include "testdata.h"

namespace gif {

// Just to make sure the headers are accessible.
TEST(GifTest, Version) { EXPECT_GE(GIFLIB_MAJOR, 5); }

TEST(GifTest, ReadFailed) {
  int err;
  // Given a null pointer, there is nothing to read.
  EXPECT_EQ(DGifOpen(nullptr, read_span, &err), nullptr);
  EXPECT_EQ(err, D_GIF_ERR_READ_FAILED);
}

TEST(GifTest, ReadSucceeded) {
  auto f = read_file("src/gif/testdata/64x64.gif");
  auto span = std::make_unique<std::span<uint8_t>>(f);
  int err;

  GifFileType *gif = DGifOpen(&span, read_span, &err);
  ASSERT_NE(gif, nullptr);

  EXPECT_EQ(DGifSlurp(gif), GIF_OK);
  EXPECT_EQ(DGifCloseFile(gif, &err), GIF_OK);
  EXPECT_EQ(err, D_GIF_SUCCEEDED);
}

} // namespace gif
