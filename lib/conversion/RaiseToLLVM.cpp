#include "conversion/RaiseToLLVM.h"
#include "llvm/IR/IntrinsicsNVPTX.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include <iostream>

using namespace llvm;

PTXToLLVMConverter::PTXToLLVMConverter(PTXProgram &program_, LLVMContext &context_) : context(context_), program(program_) {
    module = std::make_unique<Module>(program.getName(), context);
    // TODO: Do we need to hardcode these values or can they be deduced from PTX?
    module->setDataLayout("e-i64:64-i128:128-v16:16-v32:32-n16:32:64");
    module->setTargetTriple("nvptx-nvidia-cuda");
    typeInferrer = std::make_unique<PTXTypeInference>(context);
}

llvm::Type *PTXToLLVMConverter::mapType(std::string_view str) {
  IRBuilder<> Builder(context);
  if (str.find("ptr") != std::string::npos) {
    return Builder.getPtrTy();
  }
  assert("Unimplemented");
  return nullptr;
}

void PTXToLLVMConverter::AddFunction(PTXKernel &kernel) {
  typeInferrer->doTypeInference(kernel.getBody());
  auto retTy = Type::getVoidTy(context);
  // Require type annotations for all arguments
  std::vector<llvm::Type *> argTypes;
  for (auto arg : kernel.getArguments()) {
    auto argType = typeInferrer->getType(arg.getSymbol());
    if (argType) {
      argTypes.push_back(mapType(*argType));
    }
  }
  auto FuncTy = FunctionType::get(retTy, argTypes, false);
  Function *F = Function::Create(FuncTy,
                                 Function::ExternalLinkage,
                                 kernel.getName(),
                                 module.get());
  // TODO: Are all of these required?
  F->addFnAttr(Attribute::Convergent);
  F->addFnAttr(Attribute::MustProgress);
  F->addFnAttr(Attribute::NoInline);
  F->addFnAttr(Attribute::NoRecurse);
  F->addFnAttr(Attribute::NoUnwind);
  F->addFnAttr(Attribute::OptimizeNone);
  BasicBlock *BB = BasicBlock::Create(context, "", F);
  IRBuilder<> Builder(context);
  Builder.SetInsertPoint(BB);
  for (auto block : kernel.getBody().getBlocks()) {
    for (auto instr : block.getInstructions()) {
      if ((instr.getName() == "mov.u32") && (instr.getOperand(1)))  {
        CallInst *call;
        Function *FF;
        if (instr.getOperand(1)->getName() == "%ctaid.x") {
          FF = llvm::Intrinsic::getDeclaration(module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_ctaid_x);
        }
        if (instr.getOperand(1)->getName() == "%ntid.x") {
          FF = llvm::Intrinsic::getDeclaration(module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_ntid_x);
        }
        if (instr.getOperand(1)->getName() == "%tid.x") {
          FF = llvm::Intrinsic::getDeclaration(module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_tid_x);
        }
        call = Builder.CreateCall(FF);
      }
    }
  }
}

std::string PTXToLLVMConverter::printModule() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  os << *module;
  os.flush();
  return str;
}

Module *PTXToLLVMConverter::RaiseToLLVM() {
  for (auto kernel : program.getKernels()) {
    AddFunction(kernel);
  }
  return getModule();
}
