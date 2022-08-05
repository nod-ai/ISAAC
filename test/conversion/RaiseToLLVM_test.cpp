#include "gtest/gtest.h"
#include "conversion/RaiseToLLVM.h"
#include "llvm/IR/Module.h"

#define DEFINE(i, x, y) \
  { \
    PTXInstruction instr(i, {PTXOperand(x), PTXOperand(y)}); \
    block.push_back(instr); \
  }

#define DEFINE3(i, x, y, z) \
  { \
    PTXInstruction instr(i, {PTXOperand(x), PTXOperand(y), PTXOperand(z)}); \
    block.push_back(instr); \
  }


TEST(RaiseToLLVMTest, SimpleKernel) {
  llvm::LLVMContext context;
  PTXProgram program("kernel");
  std::vector<std::shared_ptr<PTXValue>> args;
  args.push_back(std::make_shared<PTXValue>("_Z3addPKfS0_Pf_param_0", PTXType("u64")));
  args.push_back(std::make_shared<PTXValue>("_Z3addPKfS0_Pf_param_1", PTXType("u64")));
  args.push_back(std::make_shared<PTXValue>("_Z3addPKfS0_Pf_param_2", PTXType("u64")));
  PTXControlFlowGraph body;
  PTXBasicBlock block;
	DEFINE("mov.u64",	"%SPL", "__local_depot6");
	DEFINE("cvta.local.u64", "%SP", "%SPL");
	DEFINE("ld.param.u64", "%rd3", "[_Z3addPKfS0_Pf_param_2]");
	DEFINE("ld.param.u64", "%rd2", "[_Z3addPKfS0_Pf_param_1]");
	DEFINE("ld.param.u64", "%rd1", "[_Z3addPKfS0_Pf_param_0]");
	DEFINE("cvta.to.global.u64", 	"%rd4", "%rd3");
	DEFINE("cvta.global.u64", 	"%rd5", "%rd4");
	DEFINE("cvta.to.global.u64", 	"%rd6", "%rd2");
	DEFINE("cvta.global.u64", 	"%rd7", "%rd6");
	DEFINE("cvta.to.global.u64", 	"%rd8", "%rd1");
	DEFINE("cvta.global.u64", 	"%rd9", "%rd8");
	DEFINE("st.u64", "[%SP+0]", "%rd9");
	DEFINE("st.u64", "[%SP+8]", "%rd7");
	DEFINE("st.u64", "[%SP+16]", "%rd5");
	DEFINE("mov.u32", "%r1", "%ctaid.x");
	DEFINE("mov.u32", "%r2", "%ntid.x");
	DEFINE3("mul.lo.s32", "%r3", "%r1", "%r2");
	DEFINE("mov.u32", "%r4", "%tid.x");
	DEFINE3("add.s32", "%r5", "%r3", "%r4");
	DEFINE("st.u32", "[%SP+24]", "%r5");
	DEFINE("ld.u64", "%rd10", "[%SP+0]");
	DEFINE("ld.u32", "%rd11", "[%SP+24]");
	DEFINE3("shl.b64", "%rd12", "%rd11", "2");
	DEFINE3("add.s64", "%rd13", "%rd10", "%rd12");;
	DEFINE("ld.f32", 	"%f1", "[%rd13]");
	DEFINE("ld.u64", 	"%rd14", "[%SP+8]");
	DEFINE3("add.s64", "%rd15", "%rd14", "%rd12");
	DEFINE("ld.f32", 	"%f2", "[%rd15]");
	DEFINE3("add.rn.f32", "%f3", "%f1", "%f2");
	DEFINE("ld.u64", 	"%rd16", "[%SP+16]");
	DEFINE3("add.s64", "%rd17", "%rd16", "%rd12");
	DEFINE("st.f32", 	"[%rd17]", "%f3");
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
    "define void @add(ptr %0, ptr %1, ptr %2) #0 {\n"
    "  %4 = alloca ptr, align 8\n"
    "  store ptr %0, ptr %4, align 8\n"
    "  %5 = alloca ptr, align 8\n"
    "  store ptr %1, ptr %5, align 8\n"
    "  %6 = alloca ptr, align 8\n"
    "  store ptr %2, ptr %6, align 8\n"
    "  %7 = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()\n"
    "  %8 = call i32 @llvm.nvvm.read.ptx.sreg.ntid.x()\n"
    "  %9 = mul i32 %7, %8\n"
    "  %10 = call i32 @llvm.nvvm.read.ptx.sreg.tid.x()\n"
    "  %11 = add i32 %9, %10\n"
    "  %12 = alloca i32, align 4\n"
    "  store i32 %11, ptr %12, align 4\n"
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
