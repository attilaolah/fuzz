#include <algorithm>
#include <cstdint>
#include <cstring>

#include <gif_lib.h>
#include <gtest/gtest.h>

#include "testdata/64x64.gif.h"
#include "utils.h"

namespace {
using ::testdata::k64x64gif;
using ::utils::read_span;
using ::utils::Span;

// Just to make sure the headers are accessible.
TEST(GifTest, Version) { EXPECT_GE(GIFLIB_MAJOR, 5); }

TEST(GifTest, ReadFailed) {
  int err;
  // Given a null pointer, there is nothing to read.
  EXPECT_EQ(DGifOpen(nullptr, read_span, &err), nullptr);
  EXPECT_EQ(err, D_GIF_ERR_READ_FAILED);
}

TEST(GifTest, ReadSucceeded) {
  Span span{.data = k64x64gif.data(), .size = k64x64gif.size()};
  int err;

  GifFileType *gif = DGifOpen(&span, read_span, &err);
  ASSERT_NE(gif, nullptr);

  EXPECT_EQ(DGifSlurp(gif), GIF_OK);
  EXPECT_EQ(DGifCloseFile(gif, &err), GIF_OK);
  EXPECT_EQ(err, D_GIF_SUCCEEDED);
}

} // namespace
