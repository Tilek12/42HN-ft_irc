#ifndef CHANNELHELPERFCTS_HPP
#define CHANNELHELPERFCTS_HPP

#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> parseCommaString(std::string commaStr);
void printVec(const std::vector<std::string>& vec);
void removeEntryFromVec(std::vector<std::string>& users, const std::string& userToRemove);

#endif
