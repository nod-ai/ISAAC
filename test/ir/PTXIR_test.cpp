#include "ir/PTXIR.h"
#include "gtest/gtest.h"

TEST(PTXIRTest, PTXOperand) {
  EXPECT_NO_THROW(std::make_unique<PTXOperand>("%rd2"));
}
