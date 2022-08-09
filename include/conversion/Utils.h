#pragma once

#include <vector>
#include <string_view>

namespace utils {

std::vector<std::string_view> tokenize(std::string_view inst);
std::string_view getPrefix(std::vector<std::string_view> &tokens);
std::string_view getInstrType(std::vector<std::string_view> &tokens);
bool isLoadInstruction(std::vector<std::string_view> &tokens);
bool isShlInstruction(std::vector<std::string_view> &tokens);
bool isStoreInstruction(std::vector<std::string_view> &tokens);
bool isConvertInstruction(std::vector<std::string_view> &tokens);
bool isBinaryInstruction(std::vector<std::string_view> &tokens);
bool isMoveInstruction(std::vector<std::string_view> &tokens);
bool isMulInstruction(std::vector<std::string_view> &tokens);
std::string_view getAddress(std::string_view str);
bool isDereference(std::string_view str);
std::string_view getStateSpace(std::vector<std::string_view> &tokens);
std::string_view getInstrType(std::vector<std::string_view> &tokens);
std::string_view getInstrMode(std::vector<std::string_view> &tokens);
bool isAddInstruction(std::vector<std::string_view> &tokens);
unsigned int getInstrBitWidth(std::vector<std::string_view> &tokens);

}
