#include "conversion/RaiseToLLVM.h"
#include "conversion/Utils.h"
#include "llvm/IR/IntrinsicsNVPTX.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include <iostream>

using namespace llvm;

PTXToLLVMConverter::PTXToLLVMConverter(PTXProgram &program_,
                                       LLVMContext &context_)
    : context(context_), program(program_) {
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

std::unordered_map<std::string_view, std::string_view>
PTXToLLVMConverter::propagateKernelArgs(PTXKernel &kernel) {
  auto body = kernel.getBody();
  std::unordered_map<std::string_view, std::string_view> clones;
  std::function<void(std::string_view)> findClones;
  auto symbolTable = body.getSymbolTable();

  findClones = [&clones, &body, &findClones](std::string_view sym) {
    auto value = body.lookup(sym);
    if (value) {
      for (auto op : (*value)->getUses()) {
        auto tokens = utils::tokenize(op->getName());
        bool isLoadParam = utils::isLoadInstruction(tokens) &&
                           utils::getStateSpace(tokens) == "param";
        bool isConvert = utils::isConvertInstruction(tokens);
        if (isLoadParam || isConvert) {
          auto cloneName = op->getOperand(0).getName();
          clones.insert({cloneName, sym});
          findClones(cloneName);
        }
      }
    }
  };
  for (auto arg : kernel.getArguments()) {
    findClones(arg->getSymbol());
  }
  return clones;
}

Value *PTXToLLVMConverter::findSourceFromClones(
    std::string_view name,
    std::unordered_map<std::string_view, std::string_view> &clones,
    StringMap<Value *> &symbolTable) {
  Value *val = symbolTable.lookup(name);
  if (val) {
    return val;
  }
  for (const auto &[k, v] : clones) {
    if (name == k) {
      val = findSourceFromClones(v, clones, symbolTable);
    }
  }
  return val;
}

void PTXToLLVMConverter::AddFunction(PTXKernel &kernel) {
  typeInferrer->doTypeInference(kernel.getBody());
  auto retTy = Type::getVoidTy(context);
  // Require type annotations for all arguments
  std::vector<llvm::Type *> argTypes;
  for (auto arg : kernel.getArguments()) {
    auto argType = typeInferrer->getType(arg->getSymbol());
    argTypes.push_back(mapType(argType));
  }
  auto FuncTy = FunctionType::get(retTy, argTypes, false);
  Function *F = Function::Create(FuncTy, Function::ExternalLinkage,
                                 kernel.getName(), module.get());
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

  // Add kernel args to symbol table
  auto body = kernel.getBody();
  StringMap<Value *> symbolTable;
  for (size_t i = 0; i < kernel.numArguments(); i++) {
    symbolTable.insert({kernel.getArgument(i)->getSymbol(), F->getArg(i)});
  }

  auto clones = propagateKernelArgs(kernel);

  for (auto block : kernel.getBody().getBlocks()) {
    for (auto instr : block.getInstructions()) {
      auto tokens = utils::tokenize(instr.getName());
      if (utils::isMoveInstruction(tokens)) {
        Function *FF{nullptr};
        if (instr.getOperand(1).getName() == "%ctaid.x") {
          FF = llvm::Intrinsic::getDeclaration(
              module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_ctaid_x);
        }
        if (instr.getOperand(1).getName() == "%ntid.x") {
          FF = llvm::Intrinsic::getDeclaration(
              module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_ntid_x);
        }
        if (instr.getOperand(1).getName() == "%tid.x") {
          FF = llvm::Intrinsic::getDeclaration(
              module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_tid_x);
        }
        if (FF) {
          CallInst *call = Builder.CreateCall(FF);
          symbolTable.insert({instr.getOperand(0).getName(), call});
        }
      }
      if (utils::isStoreInstruction(tokens)) {
        auto srcOperandName = instr.getOperand(1).getName();
        Value *src = symbolTable.lookup(srcOperandName);
        bool useTypeInference{false};
        if (!src) {
          // Source should already exist. If we can't find it, this might be a
          // clone
          src = findSourceFromClones(srcOperandName, clones, symbolTable);
          if (!src) {
            continue;
          }
          // If it is a clone, then we are dealing with kernel args. Rely on
          // type inference
          useTypeInference = true;
        }
        auto dstOperandName = utils::getAddress(instr.getOperand(0).getName());
        Value *dst = symbolTable.lookup(dstOperandName);
        if (!dst) {
          if (useTypeInference) {
            dst = Builder.CreateAlloca(
                mapType(typeInferrer->getType(dstOperandName)));
          } else {
            dst = Builder.CreateAlloca(src->getType());
          }
          symbolTable.insert({dstOperandName, dst});
        }
        Builder.CreateStore(src, dst);
      }
      if (utils::isLoadInstruction(tokens)) {
        if (utils::getStateSpace(tokens) == "param")
          continue;
        auto srcName = utils::getAddress(instr.getOperand(1).getName());
        Value *src = symbolTable.lookup(srcName);
        if (!src)
          continue;
        auto instrType = utils::getInstrType(tokens);
        Value *result;
        if (instrType == "u64") {
          result = Builder.CreateLoad(Builder.getPtrTy(), src);
        } else if (instrType == "u32") {
          result = Builder.CreateLoad(Builder.getInt32Ty(), src);
        } else if (instrType == "f32") {
          result = Builder.CreateLoad(Builder.getFloatTy(), src);
        }
        symbolTable.insert({instr.getOperand(0).getName(), result});
      }
      if (utils::isBinaryInstruction(tokens)) {
        Value *a = symbolTable.lookup(instr.getOperand(1).getName());
        if (!a) {
          continue;
        }
        Value *b = symbolTable.lookup(instr.getOperand(2).getName());
        if (!b) {
          continue;
        }

        // Check if this can be mapped to a GEP
        // Check for address calculation, followed by use in load
        // Alternatively address calculation,
        auto resName = instr.getOperand(0).getName();
        auto ptxval = body.lookup(resName);
        if (ptxval) {
          auto numUses = (*ptxval)->getNumUses();
          // TODO: This can be relaxed
          if (numUses == 1) {
            auto nextInst = (*ptxval)->getUses()[0];
            auto nextInstTokens = utils::tokenize(nextInst->getName());
            auto instrType = utils::getInstrType(nextInstTokens);
            if (utils::isLoadInstruction(nextInstTokens)) {
              // TODO: Add support for more types
              if (instrType == "f32") {
                Value *result = Builder.CreateGEP(Builder.getFloatTy(), a, b);
                symbolTable.insert({resName, result});
                continue;
              }
            }
            // Here check that we are storing into the address
            if (utils::isStoreInstruction(nextInstTokens)) {
              auto storeName =
                  utils::getAddress(nextInst->getOperand(0).getName());
              if (storeName == resName) {
                if (instrType == "f32") {
                  Value *result = Builder.CreateGEP(Builder.getFloatTy(), a, b);
                  symbolTable.insert({resName, result});
                  continue;
                }
              }
            }
          }
        }

        // Check if floating point add
        // TODO: Handle rounding modes
        if (utils::getInstrType(tokens) == "f32") {
          if (utils::isAddInstruction(tokens)) {
            Value *result = Builder.CreateFAdd(a, b);
            symbolTable.insert({instr.getOperand(0).getName(), result});
          }
          continue;
        }

        // If operands are 32-bit and a .lo is requested, we can just emit an
        // llvm mul
        auto checkBitWidth = [](Value *a) {
          return (a->getType()->isIntegerTy() &&
                  a->getType()->getIntegerBitWidth() == 32);
        };
        auto checkInstrType = utils::getInstrType(tokens) == "s32" ||
                              utils::getInstrType(tokens) == "u32";
        Value *result;
        if (utils::isMulInstruction(tokens)) {
          auto checkInstrMode = utils::getInstrMode(tokens) == "lo";
          if (checkInstrMode && checkInstrType && checkBitWidth(a) &&
              checkBitWidth(b))
            result = Builder.CreateMul(a, b);
        }
        if (utils::isAddInstruction(tokens)) {
          if (checkInstrType && checkBitWidth(a) && checkBitWidth(b))
            result = Builder.CreateAdd(a, b);
        }
        symbolTable.insert({instr.getOperand(0).getName(), result});
      }
      if (utils::isShlInstruction(tokens)) {
        Value *a = symbolTable.lookup(instr.getOperand(1).getName());
        if (!a) {
          continue;
        }
        auto type = a->getType();
        if (type->isIntegerTy()) {
          auto instrWidth = utils::getInstrBitWidth(tokens);
          if (instrWidth > type->getIntegerBitWidth()) {
            if (instrWidth == 64) {
              Value *result = Builder.CreateZExt(a, Builder.getInt64Ty());
              symbolTable.insert({instr.getOperand(0).getName(), result});
            }
          }
        }
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
