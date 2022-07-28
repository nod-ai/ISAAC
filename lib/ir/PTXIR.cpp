#include "ir/PTXIR.h"

PTXKernel::PTXKernel(std::string_view name_, std::vector<Value> &arguments_,
          ControlFlowGraph &body_) :
         name(name_), arguments(arguments_), body(body_) {}
