#include "conversion/TypeInference.h"
#include "conversion/Utils.h"
#include <iostream>
#include <algorithm>


void PTXTypeInference::constrainType(std::string_view name, std::string_view type) {
  if (typeMap.count(name)) {
    assert(name + "already present in type map!\n");
  }
  typeMap.insert({name, type});
}

static std::string_view getPointerType(std::string_view str) {
  if (str == "f32") return "ptr_f32";
  if (str == "ptr_f32") return "ptr_ptr_f32";
  if (str == "u32") return "ptr_u32";
  if (str == "u64") return "ptr_u64";
  assert("Not supported type" + str);
  return "";
}

static std::string_view getElementType(std::string_view str) {
  return str.substr(4, str.size() - 4);
}

static void propagateOperandTypes(std::vector<std::string_view> &tokens,
                                  std::vector<std::string_view> &operandTypes,
                                  std::vector<size_t> &constrainedIndices) {
  if (constrainedIndices.empty()) return;
  if (utils::isLoadInstruction(tokens)) {
    if (constrainedIndices[0] == 0) {
      operandTypes[1] = getPointerType(operandTypes[0]);
    } else {
      operandTypes[0] = getElementType(operandTypes[1]);
    }
  }
  if (utils::isStoreInstruction(tokens)) {
    if (constrainedIndices[0] == 0) {
      operandTypes[1] = getElementType(operandTypes[0]);
    } else {
      operandTypes[0] = getPointerType(operandTypes[1]);
    }
  }
  if (utils::isConvertInstruction(tokens) || utils::isBinaryInstruction(tokens)) {
    // Assumes all types are identical
    for (size_t i = 0; i < operandTypes.size(); i++) {
      operandTypes[i] = operandTypes[constrainedIndices[0]];
    }
  }
}

void PTXTypeInference::applyConstraints(PTXInstruction &instr) {
  auto tokens = utils::tokenize(instr.getName());
  std::function<void (size_t, std::string_view)> typeConstraint;
  std::string_view instrType = utils::getInstrType(tokens);
  std::vector<std::string_view> operandTypes;
  operandTypes.resize(instr.getNumOperands(), std::string_view());
  std::vector<size_t> constrainedIndices;
  for (size_t i = 0; i < instr.getNumOperands(); i++) {
    auto operandName = instr.getOperand(i).getName();
    if (utils::isDereference(operandName))
      operandName = utils::getAddress(operandName);
    if (typeMap.contains(operandName)) {
      operandTypes[i] = typeMap[operandName];
      constrainedIndices.push_back(i);
    }
  }
  propagateOperandTypes(tokens, operandTypes, constrainedIndices);
  if (utils::isLoadInstruction(tokens) || utils::isStoreInstruction(tokens)) {
    size_t pointerIndex = utils::isLoadInstruction(tokens) ? 1 : 0;
    size_t nonPointerIndex = pointerIndex == 0 ? 1 : 0;
    if (constrainedIndices.empty()) {
      typeConstraint = [instrType, pointerIndex, nonPointerIndex, this]
                       (size_t index, std::string_view name) {
        if (index == nonPointerIndex) {
          constrainType(name, instrType);
        }
        if (index == pointerIndex) {
          constrainType(utils::getAddress(name), getPointerType(instrType));
        }
      };
    } else {
      typeConstraint = [pointerIndex, nonPointerIndex, operandTypes, this]
                       (size_t index, std::string_view name) {
        if (index == nonPointerIndex)
            constrainType(name, operandTypes[index]);
        if (index == pointerIndex)
            constrainType(utils::getAddress(name), operandTypes[index]);
      };
    }
  }
  if (utils::isConvertInstruction(tokens) || utils::isBinaryInstruction(tokens)) {
    if (!constrainedIndices.empty()) {
      instrType = operandTypes[constrainedIndices[0]];
    }
    typeConstraint = [instrType, this](size_t index, std::string_view name) {
        constrainType(name, instrType);
    };
  }
  if (typeConstraint) {
    for (size_t i = 0; i < instr.getNumOperands(); i++) {
      typeConstraint(i, instr.getOperand(i).getName());
    }
  }
}

void PTXTypeInference::doTypeInference(PTXControlFlowGraph &cfg) {
  for (auto block : cfg.getBlocks()) {
    for (auto inst : llvm::reverse(block.getInstructions())) {
      applyConstraints(inst);
    }
  }
}

std::string_view PTXTypeInference::getType(std::string_view symbol) {
  return typeMap.at(symbol);
}
