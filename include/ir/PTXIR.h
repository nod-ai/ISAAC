#pragma once

#include <vector>
#include <string_view>
#include "ir/Value.h"

/**
 * @class Operand
 * @brief This class represents the operands of a PTX Instruction.
 *
 */
class PTXOperand {
public:
  enum Kind {
    Register,
    Immediate,
    Expression,
  };
  PTXOperand() = delete;
  PTXOperand(std::string_view name_, Kind kind_) :
    name(name_), kind(kind_) {}
  ~PTXOperand() {}

private:
  std::string_view name;
  Kind kind;
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
class BasicBlock {
  std::vector<PTXInstruction> instructions;
public:
  using Ptr = std::unique_ptr<BasicBlock>;
  BasicBlock() {}
  void push_back(PTXInstruction &instr) {
    instructions.push_back(instr);
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
class ControlFlowGraph {
  std::vector<BasicBlock> blocks;
public:
  ControlFlowGraph() {}
  void push_back(BasicBlock &block) {
    blocks.push_back(block);
  }
  const std::vector<BasicBlock> &getBlocks() const {
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
  std::vector<Value> arguments;
  ControlFlowGraph body;
public:
  PTXKernel(std::string_view name_, std::vector<Value> &arguments_,
            ControlFlowGraph &body_);
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
