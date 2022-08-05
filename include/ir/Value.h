#pragma once

#include <string_view>
#include <iostream>

class PTXInstruction;

/**
 * @class Type
 * @brief This class represents the type of a value
 *
 */
class PTXType {
  std::string_view type;
public:
  PTXType() {}
  PTXType(std::string_view type_) : type(type_) {}
};

/**
 * @class Value
 * @brief This class represents a value in the CFG
 *
 */
class PTXValue {
  std::string_view symbol;
  std::vector<PTXInstruction *> uses;
  PTXInstruction *def;
  PTXType type;
public:
  PTXValue() {}
  PTXValue(std::string_view symbol_, PTXType type_) :
    symbol(symbol_), type(type_) {}
  PTXType getType() const { return type; }
  std::string_view getSymbol() const { return symbol; }
  void addUse(PTXInstruction *instr) {
    uses.push_back(instr);
  }
  std::vector<PTXInstruction *> &getUses() {
    return uses;
  }
  size_t getNumUses() const {
    return uses.size();
  }
  void addDefiningInstruction(PTXInstruction *instr) {
    def = instr;
  }
  PTXInstruction *getDefiningInstruction() {
    return def;
  }
};
