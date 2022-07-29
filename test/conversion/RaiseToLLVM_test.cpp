#include "gtest/gtest.h"
#include "conversion/RaiseToLLVM.h"
#include "llvm/IR/Module.h"


TEST(RaiseToLLVMTest, SimpleKernel) {
  llvm::LLVMContext context;
  PTXProgram program("kernel");
  std::vector<PTXValue> args;
  PTXControlFlowGraph body;
  PTXBasicBlock block;
  PTXInstruction instr0("mov.u32", {PTXOperand("%r1"), PTXOperand("%ctaid.x")});
  PTXInstruction instr1("mov.u32", {PTXOperand("%r2"), PTXOperand("%ntid.x")});
  PTXInstruction instr2("mov.u32", {PTXOperand("%r4"), PTXOperand("%tid.x")});
  block.push_back(instr0);
  block.push_back(instr1);
  block.push_back(instr2);
  body.push_back(block);
  body.computeDefUseChain();
  PTXKernel kernel{"add", args, body};
  program.push_back(kernel);
  PTXToLLVMConverter converter(program, context);
  converter.RaiseToLLVM();
  auto moduleStr = converter.printModule();
  std::cout << moduleStr << std::endl;
  std::string expectedStr =
    "; ModuleID = 'kernel'\n"
    "source_filename = \"kernel\"\n"
    "target datalayout = \"e-i64:64-i128:128-v16:16-v32:32-n16:32:64\"\n"
    "target triple = \"nvptx-nvidia-cuda\"\n\n"
    "; Function Attrs: convergent mustprogress noinline norecurse nounwind optnone\n"
    "define void @add() #0 {\n"
    "  %1 = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()\n"
    "  %2 = call i32 @llvm.nvvm.read.ptx.sreg.ntid.x()\n"
    "  %3 = call i32 @llvm.nvvm.read.ptx.sreg.tid.x()\n"
    "}\n\n"
    "; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn\n"
    "declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.x() #1\n\n"
    "; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn\n"
    "declare i32 @llvm.nvvm.read.ptx.sreg.ntid.x() #1\n\n"
    "; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn\n"
    "declare i32 @llvm.nvvm.read.ptx.sreg.tid.x() #1\n\n"
    "attributes #0 = { convergent mustprogress noinline norecurse nounwind optnone }\n"
    "attributes #1 = { nocallback nofree nosync nounwind readnone speculatable willreturn }\n"
    ;
  EXPECT_EQ(moduleStr, expectedStr);
}
