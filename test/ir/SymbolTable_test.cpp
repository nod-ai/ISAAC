#include "ir/SymbolTable.h"
#include "gtest/gtest.h"

TEST(SymbolTableTest, AddEntry) {
  auto SymTable = std::make_unique<PTXSymbolTable>();
  EXPECT_EQ(SymTable->size(), 0);
  PTXType Type("i8");
  auto V = std::make_shared<PTXValue>("a", Type);
  SymTable->insert("a", V);
  EXPECT_EQ(SymTable->size(), 1);
}

TEST(SymbolTableTest, LookupEntry) {
  auto SymTable = std::make_unique<PTXSymbolTable>();
  PTXType Type("i8");
  auto V = std::make_shared<PTXValue>("a", Type);
  SymTable->insert("a", V);
  auto ShouldBeFound = SymTable->lookup("a");
  if (!ShouldBeFound) {
    FAIL() << "a should be in the symbol table!";
  }
  auto FoundValue = *ShouldBeFound;
  EXPECT_EQ(FoundValue->getSymbol(), V->getSymbol());
  auto ShouldNotBeFound = SymTable->lookup("b");
  if (ShouldNotBeFound) {
    FAIL() << "b should not be in the symbol table!";
  }
  EXPECT_EQ(ShouldNotBeFound, std::nullopt);
}
