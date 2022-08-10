#include "ir/PTXIR.h"
#include "conversion/Utils.h"
#include <iostream>

PTXKernel::PTXKernel(std::string_view Name,
                     std::vector<std::shared_ptr<PTXValue>> &Arguments,
                     PTXControlFlowGraph &Body)
    : name(Name), arguments(Arguments), body(Body) {}

void PTXControlFlowGraph::computeDefUseChain() {
  for (auto &Block : blocks) {
    Block.computeDefUseChain();
  }
}

void PTXBasicBlock::computeDefUseChain() {
  for (auto &Inst : instructions) {
    computeDefUseChain(Inst);
  }
}

void PTXBasicBlock::computeDefUseChain(PTXInstruction &Inst) {
  if (!symbolTable)
    return;
  for (size_t I = 0; I < Inst.getNumOperands(); I++) {
    auto Name = Inst.getOperand(I).getName();
    if (utils::isDereference(Name))
      Name = utils::getAddress(Name);
    auto Found = symbolTable->lookup(Name);
    // If we found the symbol already, it cannot be a addDefiningInstruction
    // and is just a use of the symbol
    if (Found) {
      (*Found)->addUse(&Inst);
      continue;
    }

    auto V = std::make_shared<PTXValue>(Name, Inst.getOperandType());
    // TODO:Does this work for all PTX instructions?
    if (I == 0) {
      V->addDefiningInstruction(&Inst);
    } else {
      V->addUse(&Inst);
    }
    symbolTable->insert(Name, V);
  }
}
