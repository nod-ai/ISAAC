#include "conversion/TypeInference.h"
#include "conversion/Utils.h"
#include <algorithm>
#include <iostream>

void PTXTypeInference::constrainType(std::string_view Name,
                                     std::string_view Type) {
  if (typeMap.count(Name)) {
    assert(name + "already present in type map!\n");
  }
  typeMap.insert({Name, Type});
}

static std::string_view getPointerType(std::string_view Str) {
  if (Str == "f32")
    return "ptr_f32";
  if (Str == "ptr_f32")
    return "ptr_ptr_f32";
  if (Str == "u32")
    return "ptr_u32";
  if (Str == "u64")
    return "ptr_u64";
  assert("Not supported type" + str);
  return "";
}

static std::string_view getElementType(std::string_view Str) {
  return Str.substr(4, Str.size() - 4);
}

static void propagateOperandTypes(std::vector<std::string_view> &Tokens,
                                  std::vector<std::string_view> &OperandTypes,
                                  std::vector<size_t> &ConstrainedIndices) {
  if (ConstrainedIndices.empty())
    return;
  if (utils::isLoadInstruction(Tokens)) {
    if (ConstrainedIndices[0] == 0) {
      OperandTypes[1] = getPointerType(OperandTypes[0]);
    } else {
      OperandTypes[0] = getElementType(OperandTypes[1]);
    }
  }
  if (utils::isStoreInstruction(Tokens)) {
    if (ConstrainedIndices[0] == 0) {
      OperandTypes[1] = getElementType(OperandTypes[0]);
    } else {
      OperandTypes[0] = getPointerType(OperandTypes[1]);
    }
  }
  if (utils::isConvertInstruction(Tokens) ||
      utils::isBinaryInstruction(Tokens)) {
    // Assumes all types are identical
    for (size_t I = 0; I < OperandTypes.size(); I++) {
      OperandTypes[I] = OperandTypes[ConstrainedIndices[0]];
    }
  }
}

void PTXTypeInference::applyConstraints(PTXInstruction &Instr) {
  auto Tokens = utils::tokenize(Instr.getName());
  std::function<void(size_t, std::string_view)> TypeConstraint;
  std::string_view InstrType = utils::getInstrType(Tokens);
  std::vector<std::string_view> OperandTypes;
  OperandTypes.resize(Instr.getNumOperands(), std::string_view());
  std::vector<size_t> ConstrainedIndices;
  for (size_t I = 0; I < Instr.getNumOperands(); I++) {
    auto OperandName = Instr.getOperand(I).getName();
    if (utils::isDereference(OperandName))
      OperandName = utils::getAddress(OperandName);
    if (typeMap.contains(OperandName)) {
      OperandTypes[I] = typeMap[OperandName];
      ConstrainedIndices.push_back(I);
    }
  }
  propagateOperandTypes(Tokens, OperandTypes, ConstrainedIndices);
  if (utils::isLoadInstruction(Tokens) || utils::isStoreInstruction(Tokens)) {
    size_t PointerIndex = utils::isLoadInstruction(Tokens) ? 1 : 0;
    size_t NonPointerIndex = PointerIndex == 0 ? 1 : 0;
    if (ConstrainedIndices.empty()) {
      TypeConstraint = [InstrType, PointerIndex, NonPointerIndex,
                        this](size_t Index, std::string_view Name) {
        if (Index == NonPointerIndex) {
          constrainType(Name, InstrType);
        }
        if (Index == PointerIndex) {
          constrainType(utils::getAddress(Name), getPointerType(InstrType));
        }
      };
    } else {
      TypeConstraint = [PointerIndex, NonPointerIndex, OperandTypes,
                        this](size_t Index, std::string_view Name) {
        if (Index == NonPointerIndex)
          constrainType(Name, OperandTypes[Index]);
        if (Index == PointerIndex)
          constrainType(utils::getAddress(Name), OperandTypes[Index]);
      };
    }
  }
  if (utils::isConvertInstruction(Tokens) ||
      utils::isBinaryInstruction(Tokens)) {
    if (!ConstrainedIndices.empty()) {
      InstrType = OperandTypes[ConstrainedIndices[0]];
    }
    TypeConstraint = [InstrType, this](size_t Index, std::string_view Name) {
      constrainType(Name, InstrType);
    };
  }
  if (TypeConstraint) {
    for (size_t I = 0; I < Instr.getNumOperands(); I++) {
      TypeConstraint(I, Instr.getOperand(I).getName());
    }
  }
}

void PTXTypeInference::doTypeInference(PTXControlFlowGraph &Cfg) {
  for (auto Block : Cfg.getBlocks()) {
    for (auto Inst : llvm::reverse(Block.getInstructions())) {
      applyConstraints(Inst);
    }
  }
}

std::string_view PTXTypeInference::getType(std::string_view Symbol) {
  return typeMap.at(Symbol);
}
