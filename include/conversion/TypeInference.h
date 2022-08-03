#pragma once
#include "ir/PTXIR.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <optional>

class PTXTypeInference {
public:
  PTXTypeInference() = delete;
  PTXTypeInference(llvm::LLVMContext &context_) : context(context_) {}
  void doTypeInference(PTXControlFlowGraph &cfg);
  void constrainType(std::string_view name, std::string_view type);
  void applyConstraints(PTXInstruction &instr);
  std::optional<std::string_view> getType(std::string_view symbol);
private:
  llvm::LLVMContext &context;
  std::unordered_map<std::string_view, std::string_view> typeMap;
  void substituteType();
};
