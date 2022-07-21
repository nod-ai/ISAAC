#pragma once

#include "Value.h"
#include <unordered_map>
#include <string_view>
#include <optional>

/**
 * @class SymbolTable
 * @brief This class defines the symbol table.
 *
 */
class SymbolTable {
  std::unordered_map<std::string_view, Value> table;
public:

  /**
   * @brief Looks up the symbol in the symbol table. Returns
   *        corresponding value if found, else none.
   *
   * @param symbol : Symbol to be searched
   */
  std::optional<Value> lookup(std::string_view symbol) const {
    if (table.contains(symbol)) {
      return table[symbol];
    }
    return {};
  }

  /**
   * @brief Stores the value corresponding to symbol in the
   *        symbol table.
   *
   * @param symbol : Symbol to be inserted into the table
   * @param value : Value corresponding to the symbol
   */
  void insert(std::string_view symbol, Value value) {
    table[symbol] = value;
  }

};
