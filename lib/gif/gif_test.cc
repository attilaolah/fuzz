#include <cstdint>
#include <cstring>

#include <gif_lib.h>
#include <gtest/gtest.h>

#include "testdata/64x64.gif.h"

namespace {
using ::testdata::k64x64gif;

// NOTE: We should use std::span, but
// getting it to compile with all the deps is painful.
struct Span {
  const uint8_t *data;
  size_t size;
};

auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int {
  auto src = static_cast<Span *>(gif->UserData);
  if (src == nullptr) {
    return 0;
  }

  size = std::min(static_cast<int>(src->size), size);
  std::cout << "READ: " << size << ", left: " << src->size << " bytes." << std::endl;
  std::memcpy(dst, src->data, size);
  src->data += size;
  src->size -= size;
  return size;
}

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

  EXPECT_EQ(DGifCloseFile(gif, &err), GIF_OK);
  EXPECT_EQ(err, D_GIF_SUCCEEDED);
}

} // namespace
