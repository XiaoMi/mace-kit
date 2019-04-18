#include <string>

#include <gtest/gtest.h>

TEST(StrCompare, StringEquals) {
  EXPECT_STREQ("hello", "hello");
}
