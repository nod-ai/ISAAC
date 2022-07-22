#pragma once

#include <string_view>

/**
 * @class Type
 * @brief This class represents the type of a value
 *
 */
class Type {
  std::string_view type;
public:
  Type() = delete;
  Type(std::string_view type_) : type(type_) {}
};

/**
 * @class Value
 * @brief This class represents a value in the CFG
 *
 */
class Value {
  std::string_view symbol;
  Type type;
public:
  Value() = delete;
  Value(std::string_view symbol_, Type type_) :
    symbol(symbol_), type(type_) {}
  Type getType() const { return type; }
  std::string_view getSymbol() const { return symbol; }
};
