// Simple test file to verify that the library is working.
#include <gif_lib.h>
#include <gtest/gtest.h>

TEST(GifTest, Version) {
  // Just make sure the version is accessible through the header.
  EXPECT_EQ(GIFLIB_MAJOR, 5);
  EXPECT_EQ(GIFLIB_MINOR, 2);
  EXPECT_EQ(GIFLIB_RELEASE, 1);
}

