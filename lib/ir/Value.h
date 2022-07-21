#pragma once

/**
 * @class Type
 * @brief This class represents the type of a value
 *
 */
class Type {
public:
};

/**
 * @class Value
 * @brief This class represents a value in the CFG
 *
 */
class Value {
  Type type;
public:
  Type getType() const { return type; }
};
