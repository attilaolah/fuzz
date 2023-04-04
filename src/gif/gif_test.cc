#include <algorithm>
#include <cstdint>
#include <cstring>

#include <gif_lib.h>
#include <gtest/gtest.h>

#include "span.h"
#include "testdata/64x64.gif.h"

namespace gif {

// Just to make sure the headers are accessible.
TEST(GifTest, Version) { EXPECT_GE(GIFLIB_MAJOR, 5); }

TEST(GifTest, ReadFailed) {
  int err;
  // Given a null pointer, there is nothing to read.
  EXPECT_EQ(DGifOpen(nullptr, &Span::read, &err), nullptr);
  EXPECT_EQ(err, D_GIF_ERR_READ_FAILED);
}

TEST(GifTest, ReadSucceeded) {
  Span span{
      .data = testdata::k64x64gif.data(),
      .size = testdata::k64x64gif.size(),
  };
  int err;

  GifFileType *gif = DGifOpen(&span, &Span::read, &err);
  ASSERT_NE(gif, nullptr);

  EXPECT_EQ(DGifSlurp(gif), GIF_OK);
  EXPECT_EQ(DGifCloseFile(gif, &err), GIF_OK);
  EXPECT_EQ(err, D_GIF_SUCCEEDED);
}

} // namespace gif
