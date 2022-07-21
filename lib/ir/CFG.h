#pragma once

#include <memory>

/**
 * @class BasicBlock
 * @brief This class represents a maximal length sequence
 *        of straightline code.
 *
 */
class BasicBlock {
public:
  using Ptr = std::unique_ptr<BasicBlock>;
};


/**
 * @class Graph
 * @brief This class represents the control flow graph.
 *
 */
class ControlFlowGraph {
public:
  using Ptr = std::unique_ptr<ControlFlowGraph>;
};
