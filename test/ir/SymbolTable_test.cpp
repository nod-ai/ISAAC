#include "gtest/gtest.h"
#include "SymbolTable.h"

TEST(SymbolTableTest, AddEntry) {
  auto symTable = std::make_unique<SymbolTable>();
  EXPECT_EQ(symTable->size(), 0);
  Type type("i8");
  auto v = Value("a", type);
  symTable->insert("a", v);
  EXPECT_EQ(symTable->size(), 1);
}

TEST(SymbolTableTest, LookupEntry) {
  auto symTable = std::make_unique<SymbolTable>();
  Type type("i8");
  auto v = Value("a", type);
  symTable->insert("a", v);
  auto shouldBeFound = symTable->lookup("a");
  if (!shouldBeFound) {
    FAIL() << "a should be in the symbol table!";
  }
  auto foundValue = *shouldBeFound;
  EXPECT_EQ(foundValue.getSymbol(), v.getSymbol());
  auto shouldNotBeFound = symTable->lookup("b");
  if (shouldNotBeFound) {
    FAIL() << "b should not be in the symbol table!";
  }
  EXPECT_EQ(shouldNotBeFound, std::nullopt);
}