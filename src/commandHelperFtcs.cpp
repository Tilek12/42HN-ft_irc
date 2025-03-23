#include <vector>
#include <iostream>

vector<std::string> parseCommaString(std::string commaStr)
{
	vector<std::string> vec;
	std::string word;
	std::stringstream ss(commaStr);
	while (getline(ss, word, ','))
	{
		vec.push_back(word);
	}
	return vec;
}