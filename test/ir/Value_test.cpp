#include "gtest/gtest.h"
#include "Value.h"

TEST(ValueTest, CreateValue) {
  EXPECT_NO_THROW(Value("a", Type("i8")));
}

