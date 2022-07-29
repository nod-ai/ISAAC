#pragma once

#include "ir/Value.h"
#include <unordered_map>
#include <string_view>
#include <optional>

/**
 * @class SymbolTable
 * @brief This class defines the symbol table.
 *
 */
class PTXSymbolTable {
  std::unordered_map<std::string_view, PTXValue> table;
public:

  PTXSymbolTable() {}
  ~PTXSymbolTable() {}

  /**
   * @brief Looks up the symbol in the symbol table. Returns
   *        corresponding value if found, else none.
   *
   * @param symbol : Symbol to be searched
   */
  std::optional<PTXValue> lookup(std::string_view symbol) const {
    if (table.contains(symbol)) {
      return table.at(symbol);
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
  void insert(std::string_view symbol, PTXValue value) {
    table.emplace(std::make_pair(symbol, value));
  }


  /**
   * @brief Returns the size of the symbol table.
   *
   * @return
   */
  size_t size() const {
    return table.size();
  }

};
