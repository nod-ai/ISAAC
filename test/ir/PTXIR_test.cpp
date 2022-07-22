#include "gtest/gtest.h"
#include "PTXIR.h"

TEST(PTXIRTest, PTXOperand) {
  EXPECT_NO_THROW(
    std::make_unique<PTXOperand>("%rd2", PTXOperand::Kind::Register));
}
