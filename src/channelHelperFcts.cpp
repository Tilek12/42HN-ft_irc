/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelHelperFcts.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:46:36 by llacsivy          #+#    #+#             */
/*   Updated: 2025/04/26 17:46:37 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <set>
#include <iostream>
#include <sstream>

#include "../include/channelHelperFcts.hpp"

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

void printVec(const std::vector<std::string>& vec)
{
	std::cout << "vec: " << std::endl;
    for (const auto& str : vec)
	{
        std::cout << str << std::endl;
    }
}

void removeEntryFromVec(std::vector<std::string>& users, \
	const std::string& userToRemove)
{
	auto it = std::find(users.begin(), users.end(), userToRemove);
	if (it != users.end())
	{
		users.erase(it);
	}
}
