#include "ir/Value.h"
#include "gtest/gtest.h"

TEST(ValueTest, CreateValue) { EXPECT_NO_THROW(PTXValue("a", PTXType("i8"))); }
