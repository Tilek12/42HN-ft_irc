
#pragma once

#include <iostream>

// Colors
constexpr std::string_view BLACK	= "\033[0;30m";
constexpr std::string_view RED		= "\033[0;31m";
constexpr std::string_view GREEN	= "\033[0;32m";
constexpr std::string_view YELLOW	= "\033[0;33m";
constexpr std::string_view BLUE		= "\033[0;34m";
constexpr std::string_view PURPLE	= "\033[0;35m";
constexpr std::string_view CYAN		= "\033[0;36m";
constexpr std::string_view WHITE	= "\033[0;37m";

// Bold colors
constexpr std::string_view B_BLACK	= "\033[1;30m";
constexpr std::string_view B_RED	= "\033[1;31m";
constexpr std::string_view B_GREEN	= "\033[1;32m";
constexpr std::string_view B_YELLOW	= "\033[1;33m";
constexpr std::string_view B_BLUE	= "\033[1;34m";
constexpr std::string_view B_PURPLE	= "\033[1;35m";
constexpr std::string_view B_CYAN	= "\033[1;36m";
constexpr std::string_view B_WHITE	= "\033[1;37m";

// Reset code
constexpr std::string_view RESET	= "\033[0m";
