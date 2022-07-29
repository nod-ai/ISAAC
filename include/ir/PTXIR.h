#pragma once

#include <vector>
#include <optional>
#include <string_view>
#include "ir/Value.h"
#include "ir/SymbolTable.h"

/**
 * @class Operand
 * @brief This class represents the operands of a PTX Instruction.
 *
 */
class PTXOperand {
public:
  PTXOperand() = delete;
  PTXOperand(std::string_view name_) :
    name(name_) {}
  ~PTXOperand() {}
  std::string_view getName() const { return name; }

private:
  std::string_view name;
};

/**
 * @class Instruction
 * @brief This class represents a PTX Instruction.
 *
 */
class PTXInstruction {
  std::string_view name;
  std::vector<PTXOperand> operands;
public:
  PTXInstruction() = delete;
  PTXInstruction(std::string_view name_, const std::vector<PTXOperand> &operands_) :
    name(name_), operands(operands_) {}
  ~PTXInstruction() {}
  std::string_view getName() const { return name; }
  std::optional<PTXOperand> getOperand(size_t idx) {
    if (idx < operands.size()) return operands[idx];
    return {};
  }
  std::vector<PTXOperand> & getOperands() {
    return operands;
  }
  std::string_view getOperandType() {
    return name.substr(name.size() - 3);
  }
};


/**
 * @class PTXDirective
 * @brief A PTX Directive starts with a '.'
 *
 */
class PTXDirective {
  std::string_view name;
public:
  PTXDirective() = delete;
};

/**
 * @class BasicBlock
 * @brief This class represents a maximal length sequence
 *        of straightline code.
 *
 */
class PTXBasicBlock {
  std::vector<PTXInstruction> instructions;
  PTXSymbolTable *symbolTable;
  void computeDefUseChain(PTXInstruction &inst);
public:
  using Ptr = std::unique_ptr<PTXBasicBlock>;
  PTXBasicBlock() {}
  void processInstruction(PTXInstruction &inst);
  void push_back(PTXInstruction &instr) {
    instructions.push_back(instr);
  }
  void setSymbolTable(PTXSymbolTable *symbolTable_) {
    symbolTable = symbolTable_;
  }
  void computeDefUseChain();
  size_t numInstructions() const {
    return instructions.size();
  }
  const std::vector<PTXInstruction> &getInstructions() const {
    return instructions;
  }
};


/**
 * @class Graph
 * @brief This class represents the control flow graph.
 *
 */
class PTXControlFlowGraph {
  std::vector<PTXBasicBlock> blocks;
  PTXSymbolTable symbolTable;
public:
  PTXControlFlowGraph() {}
  void push_back(PTXBasicBlock &block) {
    block.setSymbolTable(&symbolTable);
    blocks.push_back(block);
  }
  void computeDefUseChain();
  const std::vector<PTXBasicBlock> &getBlocks() const {
    return blocks;
  }
};


/**
 * @class PTXKernel
 * @brief A PTX Kernel has a name, takes in 0 or more arguments
 *        and has a body containing the computation.
 *
 */
class PTXKernel {
  std::string_view name;
  std::vector<PTXValue> arguments;
  PTXControlFlowGraph body;
public:
  PTXKernel(std::string_view name_, std::vector<PTXValue> &arguments_,
            PTXControlFlowGraph &body_);
  std::string_view getName() const { return name; }
  PTXControlFlowGraph &getBody() { return body; }
};


/**
 * @class PTXProgram
 * @brief A PTXProgram is composed of multiple kernels,
 *        global directives and variables.
 *
 */
class PTXProgram {
  std::vector<PTXKernel> kernels;
  std::string_view name;
public:
  PTXProgram(std::string_view name_) : name(name_) {}
  void push_back(PTXKernel &kernel) {
    kernels.push_back(kernel);
  }
  const std::vector<PTXKernel> &getKernels() const {
    return kernels;
  }
  std::string_view getName() const { return name; }
};
