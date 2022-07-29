#include "gtest/gtest.h"
#include "ir/Value.h"

TEST(ValueTest, CreateValue) {
  EXPECT_NO_THROW(PTXValue("a", PTXType("i8")));
}

