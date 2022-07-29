#include "ir/PTXIR.h"
#include <iostream>

PTXKernel::PTXKernel(std::string_view name_, std::vector<PTXValue> &arguments_,
          PTXControlFlowGraph &body_) :
         name(name_), arguments(arguments_), body(body_) {}

void PTXControlFlowGraph::computeDefUseChain() {
  for (auto block : blocks) {
    block.computeDefUseChain();
  }
}

void PTXBasicBlock::computeDefUseChain() {
  for (auto inst : instructions) {
    computeDefUseChain(inst);
  }
}

void PTXBasicBlock::computeDefUseChain(PTXInstruction &inst) {
  if (!symbolTable)
    return;
  int i = 0;
  for (auto operand : inst.getOperands()) {
    auto name = operand.getName();
    auto found = symbolTable->lookup(name);
    PTXValue v;
    if (!found) {
      v = PTXValue(name, inst.getOperandType());
    } else {
      v = *found;
    }
    // TODO:Does this work for all PTX instructions?
    if (i == 0) {
      v.addDefiningInstruction(&inst);
    } else {
      v.addUse(&inst);
    }
    if (!found)
      symbolTable->insert(name, v);
    i++;
  }
}
