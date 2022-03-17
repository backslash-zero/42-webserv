#pragma once

#include <string>
#include <iostream>

#include "cosmetics.hpp"

void printError(const std::string &str)
{
	std::cerr << RED << str << RESET << std::endl;
}
void printError(const std::string &str, const std::string &str2)
{
	std::cerr << RED << str << RESET << str2 << std::endl;
}

void printBoldError(const std::string &str)
{
	std::cerr << BOLDRED << str << RESET << std::endl;
}