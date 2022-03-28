#ifndef PARSER_HPP
# define PARSER_HPP

#include "webserv.hpp"


struct SingleSetting {
	public:

	std::string		section;
	std::string		key;
	std::string		value;
};

class Parser {
	
	public:

	std::vector<std::string>		getLineFile(std::string filename);
	std::string						StripLine(std::string in);
	std::vector<std::string>		ParseSection(std::string &src);
	std::vector<std::string>		ParseFile(std::string filename);
};

namespace constants {
	const char SPACE = ' ';
	const char TAB = '\t';
	const char HYPHEN = '-';
	const char UNDERSCORE = '_';
	const char EQUALS = '=';
	const char SEMICOLON = ';';
	const char SECTION_START = '{';
	const char SECTION_END = '}';
	const char QUOTE = '\"';
	const char DECIMAL = '.';
	const char COMMA = ',';
	const char COMMENT_DELIM = '#';
}



#endif
