#pragma once

#include <vector>
#include <string_view>
#include "Value.h"
#include "CFG.h"

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
 * @class PTXKernel
 * @brief A PTX Kernel has a name, takes in 0 or more arguments
 *        and has a body containing the computation.
 *
 */
class PTXKernel {
  std::string_view name;
  std::vector<Value> arguments;
  ControlFlowGraph::Ptr body;
public:
  using Ptr = std::unique_ptr<PTXKernel>;

};


/**
 * @class PTXProgram
 * @brief A PTXProgram is composed of multiple kernels,
 *        global directives and variables.
 *
 */
class PTXProgram {
  std::vector<PTXKernel::Ptr> kernels;
  std::string_view name;
public:
  PTXProgram(std::string_view name_) : name(name_) {}
  void push_back(PTXKernel::Ptr kernel) {
    kernels.push_back(kernel);
  }
};
