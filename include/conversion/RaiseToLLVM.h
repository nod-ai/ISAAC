#pragma once

#include "ir/PTXIR.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "conversion/TypeInference.h"

class PTXToLLVMConverter {
  std::unique_ptr<llvm::Module> module;
  llvm::LLVMContext &context;
  PTXProgram &program;
  std::unique_ptr<PTXTypeInference> typeInferrer;
  void performTypeInference(PTXBasicBlock &block);
  llvm::Type *mapType(std::string_view str);
public:
  PTXToLLVMConverter(PTXProgram &program, llvm::LLVMContext &context);
  ~PTXToLLVMConverter() {}
  void AddFunction(PTXKernel &kernel);
  std::string printModule() const;
  llvm::Module *getModule() const {
    return module.get();
  }
  llvm::Module *RaiseToLLVM();
};

