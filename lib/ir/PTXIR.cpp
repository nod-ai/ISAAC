#include "ir/PTXIR.h"
#include "conversion/Utils.h"
#include <iostream>

PTXKernel::PTXKernel(std::string_view name_, std::vector<std::shared_ptr<PTXValue>> &arguments_,
          PTXControlFlowGraph &body_) :
         name(name_), arguments(arguments_), body(body_) {}

void PTXControlFlowGraph::computeDefUseChain() {
  for (auto &block : blocks) {
    block.computeDefUseChain();
  }
}

void PTXBasicBlock::computeDefUseChain() {
  for (auto &inst : instructions) {
    computeDefUseChain(inst);
  }
}

void PTXBasicBlock::computeDefUseChain(PTXInstruction &inst) {
  if (!symbolTable)
    return;
  for (size_t i = 0; i < inst.getNumOperands(); i++) {
    auto name = inst.getOperand(i).getName();
    if (utils::isDereference(name))
      name = utils::getAddress(name);
    auto found = symbolTable->lookup(name);
    // If we found the symbol already, it cannot be a addDefiningInstruction
    // and is just a use of the symbol
    if (found) {
      (*found)->addUse(&inst);
      continue;
    }

    auto v = std::make_shared<PTXValue>(name, inst.getOperandType());
    // TODO:Does this work for all PTX instructions?
    if (i == 0) {
      v->addDefiningInstruction(&inst);
    } else {
      v->addUse(&inst);
    }
    symbolTable->insert(name, v);
  }
}
