#include "gtest/gtest.h"
#include "conversion/RaiseToLLVM.h"
#include "llvm/IR/Module.h"


TEST(RaiseToLLVMTest, SimpleTest) {
  llvm::LLVMContext context;
  PTXProgram program("kernel");
  std::vector<Value> args;
  ControlFlowGraph body;
  PTXKernel kernel{"add", args, body};
  program.push_back(kernel);
  PTXToLLVMConverter converter("kernel", context);
  converter.RaiseToLLVM(program, context);
  auto moduleStr = converter.printModule();
  std::string expectedStr =
    "; ModuleID = 'kernel'\n"
    "source_filename = \"kernel\"\n"
    "target datalayout = \"e-i64:64-i128:128-v16:16-v32:32-n16:32:64\"\n"
    "target triple = \"nvptx-nvidia-cuda\"\n";
  EXPECT_EQ(moduleStr, expectedStr);
}
