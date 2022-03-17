#pragma once

#include <string>
#include <iostream>

#include "cosmetics.hpp"

void printError(const std::string &str)
{
	std::cerr << RED << str << RESET << std::endl;
}

void printBoldError(const std::string &str)
{
	std::cerr << BOLDRED << str << RESET << std::endl;
}