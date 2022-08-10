#include "conversion/Utils.h"
#include <cassert>
#include <sstream>

namespace utils {

std::vector<std::string_view> tokenize(std::string_view inst) {
  std::vector<std::string_view> tokens;
  size_t begin{0}, end{0};
  for (size_t i = 0; i < inst.size(); i++) {
    if (inst[i] == '.') {
      end = i;
      tokens.push_back(inst.substr(begin, end - begin));
      begin = end + 1;
    }
    if (i == inst.size() - 1) {
      tokens.push_back(inst.substr(begin, inst.size() - begin));
    }
  }
  return tokens;
}

std::string_view getPrefix(std::vector<std::string_view> &tokens) {
  return tokens[0];
}

std::string_view getStateSpace(std::vector<std::string_view> &tokens) {
  assert(tokens.size() >= 2);
  std::vector<std::string_view> spaces{"param", "global", "local", "shared",
                                       "const", "sreg",   "reg"};
  for (auto space : spaces) {
    if (tokens[1] == space)
      return space;
  }
  return "generic";
}

// Assumes last token can be used to determine the type
std::string_view getInstrType(std::vector<std::string_view> &tokens) {
  return tokens.back();
}

unsigned int getInstrBitWidth(std::vector<std::string_view> &tokens) {
  std::string s(utils::getInstrType(tokens).substr(1));
  std::stringstream ss(s);
  unsigned int instrWidth;
  ss >> instrWidth;
  return instrWidth;
}

std::string_view getInstrMode(std::vector<std::string_view> &tokens) {
  // For instructions with 3 parts, return the middle token
  if (tokens.size() == 3)
    return tokens[1];
  return "";
}

bool isMoveInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "mov";
}

bool isMulInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "mul";
}

bool isAddInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "add";
}

bool isShlInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "shl";
}

bool isLoadInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "ld";
}

bool isStoreInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "st";
}

bool isConvertInstruction(std::vector<std::string_view> &tokens) {
  return getPrefix(tokens) == "cvta";
}

bool isBinaryInstruction(std::vector<std::string_view> &tokens) {
  auto prefix = getPrefix(tokens);
  return ((prefix == "mul") || (prefix == "add"));
}

std::string_view getAddress(std::string_view str) {
  return str.substr(1, str.size() - 2);
}

bool isDereference(std::string_view str) {
  return (str[0] == '[') && (str.back() == ']');
}

} // namespace utils
