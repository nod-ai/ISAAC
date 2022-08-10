#include "conversion/RaiseToLLVM.h"
#include "conversion/Utils.h"
#include "llvm/IR/IntrinsicsNVPTX.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include <iostream>

using namespace llvm;

PTXToLLVMConverter::PTXToLLVMConverter(PTXProgram &Program,
                                       LLVMContext &Context)
    : context(Context), program(Program) {
  module = std::make_unique<Module>(program.getName(), context);
  // TODO: Do we need to hardcode these values or can they be deduced from PTX?
  module->setDataLayout("e-i64:64-i128:128-v16:16-v32:32-n16:32:64");
  module->setTargetTriple("nvptx-nvidia-cuda");
  typeInferrer = std::make_unique<PTXTypeInference>(context);
}

llvm::Type *PTXToLLVMConverter::mapType(std::string_view Str) {
  IRBuilder<> Builder(context);
  if (Str.find("ptr") != std::string::npos) {
    return Builder.getPtrTy();
  }
  assert("Unimplemented");
  return nullptr;
}

std::unordered_map<std::string_view, std::string_view>
PTXToLLVMConverter::propagateKernelArgs(PTXKernel &Kernel) {
  auto Body = Kernel.getBody();
  std::unordered_map<std::string_view, std::string_view> Clones;
  std::function<void(std::string_view)> FindClones;
  auto SymbolTable = Body.getSymbolTable();

  FindClones = [&Clones, &Body, &FindClones](std::string_view Sym) {
    auto Value = Body.lookup(Sym);
    if (Value) {
      for (auto Op : (*Value)->getUses()) {
        auto Tokens = utils::tokenize(Op->getName());
        bool IsLoadParam = utils::isLoadInstruction(Tokens) &&
                           utils::getStateSpace(Tokens) == "param";
        bool IsConvert = utils::isConvertInstruction(Tokens);
        if (IsLoadParam || IsConvert) {
          auto CloneName = Op->getOperand(0).getName();
          Clones.insert({CloneName, Sym});
          FindClones(CloneName);
        }
      }
    }
  };
  for (auto Arg : Kernel.getArguments()) {
    FindClones(Arg->getSymbol());
  }
  return Clones;
}

Value *PTXToLLVMConverter::findSourceFromClones(
    std::string_view Name,
    std::unordered_map<std::string_view, std::string_view> &Clones,
    StringMap<Value *> &SymbolTable) {
  Value *Val = SymbolTable.lookup(Name);
  if (Val) {
    return Val;
  }
  for (const auto &[k, v] : Clones) {
    if (Name == k) {
      Val = findSourceFromClones(v, Clones, SymbolTable);
    }
  }
  return Val;
}

void PTXToLLVMConverter::AddFunction(PTXKernel &Kernel) {
  typeInferrer->doTypeInference(Kernel.getBody());
  auto RetTy = Type::getVoidTy(context);
  // Require type annotations for all arguments
  std::vector<llvm::Type *> ArgTypes;
  for (auto Arg : Kernel.getArguments()) {
    auto ArgType = typeInferrer->getType(Arg->getSymbol());
    ArgTypes.push_back(mapType(ArgType));
  }
  auto FuncTy = FunctionType::get(RetTy, ArgTypes, false);
  Function *F = Function::Create(FuncTy, Function::ExternalLinkage,
                                 Kernel.getName(), module.get());
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
  auto Body = Kernel.getBody();
  StringMap<Value *> SymbolTable;
  for (size_t I = 0; I < Kernel.numArguments(); I++) {
    SymbolTable.insert({Kernel.getArgument(I)->getSymbol(), F->getArg(I)});
  }

  auto Clones = propagateKernelArgs(Kernel);

  for (auto Block : Kernel.getBody().getBlocks()) {
    for (auto Instr : Block.getInstructions()) {
      auto Tokens = utils::tokenize(Instr.getName());
      if (utils::isMoveInstruction(Tokens)) {
        Function *FF{nullptr};
        if (Instr.getOperand(1).getName() == "%ctaid.x") {
          FF = llvm::Intrinsic::getDeclaration(
              module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_ctaid_x);
        }
        if (Instr.getOperand(1).getName() == "%ntid.x") {
          FF = llvm::Intrinsic::getDeclaration(
              module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_ntid_x);
        }
        if (Instr.getOperand(1).getName() == "%tid.x") {
          FF = llvm::Intrinsic::getDeclaration(
              module.get(), llvm::Intrinsic::nvvm_read_ptx_sreg_tid_x);
        }
        if (FF) {
          CallInst *Call = Builder.CreateCall(FF);
          SymbolTable.insert({Instr.getOperand(0).getName(), Call});
        }
      }
      if (utils::isStoreInstruction(Tokens)) {
        auto SrcOperandName = Instr.getOperand(1).getName();
        Value *Src = SymbolTable.lookup(SrcOperandName);
        bool UseTypeInference{false};
        if (!Src) {
          // Source should already exist. If we can't find it, this might be a
          // clone
          Src = findSourceFromClones(SrcOperandName, Clones, SymbolTable);
          if (!Src) {
            continue;
          }
          // If it is a clone, then we are dealing with kernel args. Rely on
          // type inference
          UseTypeInference = true;
        }
        auto DstOperandName = utils::getAddress(Instr.getOperand(0).getName());
        Value *Dst = SymbolTable.lookup(DstOperandName);
        if (!Dst) {
          if (UseTypeInference) {
            Dst = Builder.CreateAlloca(
                mapType(typeInferrer->getType(DstOperandName)));
          } else {
            Dst = Builder.CreateAlloca(Src->getType());
          }
          SymbolTable.insert({DstOperandName, Dst});
        }
        Builder.CreateStore(Src, Dst);
      }
      if (utils::isLoadInstruction(Tokens)) {
        if (utils::getStateSpace(Tokens) == "param")
          continue;
        auto SrcName = utils::getAddress(Instr.getOperand(1).getName());
        Value *Src = SymbolTable.lookup(SrcName);
        if (!Src)
          continue;
        auto InstrType = utils::getInstrType(Tokens);
        Value *Result;
        if (InstrType == "u64") {
          Result = Builder.CreateLoad(Builder.getPtrTy(), Src);
        } else if (InstrType == "u32") {
          Result = Builder.CreateLoad(Builder.getInt32Ty(), Src);
        } else if (InstrType == "f32") {
          Result = Builder.CreateLoad(Builder.getFloatTy(), Src);
        }
        SymbolTable.insert({Instr.getOperand(0).getName(), Result});
      }
      if (utils::isBinaryInstruction(Tokens)) {
        Value *A = SymbolTable.lookup(Instr.getOperand(1).getName());
        if (!A) {
          continue;
        }
        Value *B = SymbolTable.lookup(Instr.getOperand(2).getName());
        if (!B) {
          continue;
        }

        // Check if this can be mapped to a GEP
        // Check for address calculation, followed by use in load
        // Alternatively address calculation,
        auto ResName = Instr.getOperand(0).getName();
        auto Ptxval = Body.lookup(ResName);
        if (Ptxval) {
          auto NumUses = (*Ptxval)->getNumUses();
          // TODO: This can be relaxed
          if (NumUses == 1) {
            auto NextInst = (*Ptxval)->getUses()[0];
            auto NextInstTokens = utils::tokenize(NextInst->getName());
            auto InstrType = utils::getInstrType(NextInstTokens);
            if (utils::isLoadInstruction(NextInstTokens)) {
              // TODO: Add support for more types
              if (InstrType == "f32") {
                Value *Result = Builder.CreateGEP(Builder.getFloatTy(), A, B);
                SymbolTable.insert({ResName, Result});
                continue;
              }
            }
            // Here check that we are storing into the address
            if (utils::isStoreInstruction(NextInstTokens)) {
              auto StoreName =
                  utils::getAddress(NextInst->getOperand(0).getName());
              if (StoreName == ResName) {
                if (InstrType == "f32") {
                  Value *Result = Builder.CreateGEP(Builder.getFloatTy(), A, B);
                  SymbolTable.insert({ResName, Result});
                  continue;
                }
              }
            }
          }
        }

        // Check if floating point add
        // TODO: Handle rounding modes
        if (utils::getInstrType(Tokens) == "f32") {
          if (utils::isAddInstruction(Tokens)) {
            Value *Result = Builder.CreateFAdd(A, B);
            SymbolTable.insert({Instr.getOperand(0).getName(), Result});
          }
          continue;
        }

        // If operands are 32-bit and a .lo is requested, we can just emit an
        // llvm mul
        auto CheckBitWidth = [](Value *A) {
          return (A->getType()->isIntegerTy() &&
                  A->getType()->getIntegerBitWidth() == 32);
        };
        auto CheckInstrType = utils::getInstrType(Tokens) == "s32" ||
                              utils::getInstrType(Tokens) == "u32";
        Value *Result;
        if (utils::isMulInstruction(Tokens)) {
          auto CheckInstrMode = utils::getInstrMode(Tokens) == "lo";
          if (CheckInstrMode && CheckInstrType && CheckBitWidth(A) &&
              CheckBitWidth(B))
            Result = Builder.CreateMul(A, B);
        }
        if (utils::isAddInstruction(Tokens)) {
          if (CheckInstrType && CheckBitWidth(A) && CheckBitWidth(B))
            Result = Builder.CreateAdd(A, B);
        }
        SymbolTable.insert({Instr.getOperand(0).getName(), Result});
      }
      if (utils::isShlInstruction(Tokens)) {
        Value *A = SymbolTable.lookup(Instr.getOperand(1).getName());
        if (!A) {
          continue;
        }
        auto Type = A->getType();
        if (Type->isIntegerTy()) {
          auto InstrWidth = utils::getInstrBitWidth(Tokens);
          if (InstrWidth > Type->getIntegerBitWidth()) {
            if (InstrWidth == 64) {
              Value *Result = Builder.CreateZExt(A, Builder.getInt64Ty());
              SymbolTable.insert({Instr.getOperand(0).getName(), Result});
            }
          }
        }
      }
    }
  }
}

std::string PTXToLLVMConverter::printModule() const {
  std::string Str;
  llvm::raw_string_ostream Os(Str);
  Os << *module;
  Os.flush();
  return Str;
}

Module *PTXToLLVMConverter::RaiseToLLVM() {
  for (auto Kernel : program.getKernels()) {
    AddFunction(Kernel);
  }
  return getModule();
}
