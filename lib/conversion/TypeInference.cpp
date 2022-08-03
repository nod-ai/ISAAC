#include "conversion/TypeInference.h"
#include <iostream>
#include <algorithm>

static std::vector<std::string_view> tokenize(std::string_view inst) {
  std::vector<std::string_view> tokens;
  size_t begin{0}, end{0};
  for (size_t i = 0; i < inst.size(); i++) {
    if (inst[i] == '.') {
      end = i;
      tokens.push_back(inst.substr(begin, end - begin));
      begin = end + 1;
    }
    if (i == inst.size() - 1) {
      tokens.push_back(inst.substr(begin, inst.size() -  begin));
    }
  }
  return tokens;
}

static std::string_view getPrefix(std::vector<std::string_view> &tokens) {
  return tokens[0];
}

// Assumes last token can be used to determine the type
static inline std::string_view getInstrType(std::vector<std::string_view> &tokens) {
  return tokens.back();
}

static inline bool isLoadInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "ld";
}

static inline bool isStoreInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "st";
}

static inline bool isConvertInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "cvta";
}

static inline bool isBinaryInstruction(std::vector<std::string_view> &tokens) {
  auto prefix = getPrefix(tokens);
  return ((prefix == "mul") || (prefix == "add") || (prefix == "shl"));
}

void PTXTypeInference::constrainType(std::string_view name, std::string_view type) {
  if (typeMap.count(name)) {
    assert(name + "already present in type map!\n");
  }
  typeMap.insert({name, type});
}

static inline std::string_view getAddress(std::string_view str) {
  return str.substr(1, str.size() - 2);
}

static inline bool isDereference(std::string_view str) {
  return (str[0] == '[') && (str.back() == ']');
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
  if (isLoadInstruction(tokens)) {
    if (constrainedIndices[0] == 0) {
      operandTypes[1] = getPointerType(operandTypes[0]);
    } else {
      operandTypes[0] = getElementType(operandTypes[1]);
    }
  }
  if (isStoreInstruction(tokens)) {
    if (constrainedIndices[0] == 0) {
      operandTypes[1] = getElementType(operandTypes[0]);
    } else {
      operandTypes[0] = getPointerType(operandTypes[1]);
    }
  }
  if (isConvertInstruction(tokens) || isBinaryInstruction(tokens)) {
    // Assumes all types are identical
    for (size_t i = 0; i < operandTypes.size(); i++) {
      operandTypes[i] = operandTypes[constrainedIndices[0]];
    }
  }
}

void PTXTypeInference::applyConstraints(PTXInstruction &instr) {
  auto tokens = tokenize(instr.getName());
  std::function<void (size_t, std::string_view)> typeConstraint;
  std::string_view instrType = getInstrType(tokens);
  std::vector<std::string_view> operandTypes;
  operandTypes.resize(instr.getNumOperands(), std::string_view());
  std::vector<size_t> constrainedIndices;
  for (size_t i = 0; i < instr.getNumOperands(); i++) {
    if (instr.getOperand(i)) {
      auto operandName = instr.getOperand(i)->getName();
      if (isDereference(operandName))
        operandName = getAddress(operandName);
      if (typeMap.contains(operandName)) {
        operandTypes[i] = typeMap[operandName];
        constrainedIndices.push_back(i);
      }
    }
  }
  propagateOperandTypes(tokens, operandTypes, constrainedIndices);
  if (isLoadInstruction(tokens) || isStoreInstruction(tokens)) {
    size_t pointerIndex = isLoadInstruction(tokens) ? 1 : 0;
    size_t nonPointerIndex = pointerIndex == 0 ? 1 : 0;
    if (constrainedIndices.empty()) {
      typeConstraint = [instrType, pointerIndex, nonPointerIndex, this]
                       (size_t index, std::string_view name) {
        if (index == nonPointerIndex) {
          constrainType(name, instrType);
        }
        if (index == pointerIndex) {
          constrainType(getAddress(name), getPointerType(instrType));
        }
      };
    } else {
      typeConstraint = [pointerIndex, nonPointerIndex, operandTypes, this]
                       (size_t index, std::string_view name) {
        if (index == nonPointerIndex)
            constrainType(name, operandTypes[index]);
        if (index == pointerIndex)
            constrainType(getAddress(name), operandTypes[index]);
      };
    }
  }
  if (isConvertInstruction(tokens) || isBinaryInstruction(tokens)) {
    if (!constrainedIndices.empty()) {
      instrType = operandTypes[constrainedIndices[0]];
    }
    typeConstraint = [instrType, this](size_t index, std::string_view name) {
        constrainType(name, instrType);
    };
  }
  if (typeConstraint) {
    for (size_t i = 0; i < instr.getNumOperands(); i++) {
      if (instr.getOperand(i))
        typeConstraint(i, instr.getOperand(i)->getName());
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

std::optional<std::string_view> PTXTypeInference::getType(std::string_view symbol) {
  if (typeMap.contains(symbol)) {
    return typeMap.at(symbol);
  }
  return {};
}
