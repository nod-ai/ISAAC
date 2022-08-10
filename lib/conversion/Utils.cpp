#include "conversion/Utils.h"
#include <cassert>
#include <sstream>

namespace utils {

std::vector<std::string_view> tokenize(std::string_view Inst) {
  std::vector<std::string_view> Tokens;
  size_t Begin{0}, End{0};
  for (size_t I = 0; I < Inst.size(); I++) {
    if (Inst[I] == '.') {
      End = I;
      Tokens.push_back(Inst.substr(Begin, End - Begin));
      Begin = End + 1;
    }
    if (I == Inst.size() - 1) {
      Tokens.push_back(Inst.substr(Begin, Inst.size() - Begin));
    }
  }
  return Tokens;
}

std::string_view getPrefix(std::vector<std::string_view> &Tokens) {
  return Tokens[0];
}

std::string_view getStateSpace(std::vector<std::string_view> &Tokens) {
  assert(tokens.size() >= 2);
  std::vector<std::string_view> Spaces{"param", "global", "local", "shared",
                                       "const", "sreg",   "reg"};
  for (auto Space : Spaces) {
    if (Tokens[1] == Space)
      return Space;
  }
  return "generic";
}

// Assumes last token can be used to determine the type
std::string_view getInstrType(std::vector<std::string_view> &Tokens) {
  return Tokens.back();
}

unsigned int getInstrBitWidth(std::vector<std::string_view> &Tokens) {
  std::string S(utils::getInstrType(Tokens).substr(1));
  std::stringstream Ss(S);
  unsigned int InstrWidth;
  Ss >> InstrWidth;
  return InstrWidth;
}

std::string_view getInstrMode(std::vector<std::string_view> &Tokens) {
  // For instructions with 3 parts, return the middle token
  if (Tokens.size() == 3)
    return Tokens[1];
  return "";
}

bool isMoveInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "mov";
}

bool isMulInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "mul";
}

bool isAddInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "add";
}

bool isShlInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "shl";
}

bool isLoadInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "ld";
}

bool isStoreInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "st";
}

bool isConvertInstruction(std::vector<std::string_view> &Tokens) {
  return getPrefix(Tokens) == "cvta";
}

bool isBinaryInstruction(std::vector<std::string_view> &Tokens) {
  auto Prefix = getPrefix(Tokens);
  return ((Prefix == "mul") || (Prefix == "add"));
}

std::string_view getAddress(std::string_view Str) {
  return Str.substr(1, Str.size() - 2);
}

bool isDereference(std::string_view Str) {
  return (Str[0] == '[') && (Str.back() == ']');
}

} // namespace utils
