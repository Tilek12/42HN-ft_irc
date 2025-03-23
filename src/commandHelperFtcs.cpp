#include <vector>
#include <iostream>
#include <sstream>

#include "../include/commandHelperFtcs.hpp"

std::vector<std::string> parseCommaString(std::string commaStr)
{
	std::vector<std::string> vec;
	std::string word;
	std::stringstream ss(commaStr);
	while (getline(ss, word, ','))
	{
		vec.push_back(word);
	}
	return vec;
}