#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>

#include "../incs/conf_parser.hpp"


std::vector<std::string>		Parser::getLineFile(std::string filename) {
	std::ifstream				file(filename.c_str());
	std::vector<std::string>	lines;
	std::string					line;

	if (!file)
		throw std::runtime_error("file open failure");
	while (std::getline(file, line))
		lines.push_back(line);
	return lines;
}

std::string						Parser::StripLine(std::string in) {
	std::string	out = "";
	bool		inQuotes = false;
	bool		dsp = false;

	std::string::iterator ite = in.end();
	for (std::string::iterator it = in.begin(); it != ite; it++) {
		if ((*it == constants::SPACE || *it == constants::TAB) && !dsp) {
			continue;
		}
		dsp = true;
		if (*it == constants::COMMENT_DELIM && !inQuotes) {
			break;
		}
		if (*it == constants::QUOTE) {
			inQuotes = !inQuotes;
			out += constants::QUOTE;
			continue;
		}
		out += *it;
	}
	return out;
}

std::vector<std::string>		Parser::ParseSection(std::string &src) {
	std::vector<std::string>	token;
	std::string::iterator it = src.begin();
	std::string::iterator ite = src.end();
	size_t	i = 0;
	size_t	j = 0;
	bool		inQuotes = false;

	while (it != ite) {
		if (*it == constants::SECTION_START || *it == constants::SECTION_END || *it == constants::SEMICOLON) {
			token.push_back(src.substr(i, 1));
			it++;
			i++;
			j++;
		}
		while (it != ite && (*it == constants::SPACE || *it == constants::TAB)) {
			it++;
			i++;
			j++;
		}
		if (*it == constants::QUOTE) {
			inQuotes = !inQuotes;
			it++;
			j++;
			while (it != ite && inQuotes) {
				if (*it == constants::QUOTE)
					inQuotes = !inQuotes;
				it++;
				j++;
			}
			if (j - i > 0)
			token.push_back(src.substr(i, j - i));
			i = j;
		}
		while (it != ite && *it != constants::SPACE && *it != constants::TAB && *it != constants::SECTION_START &&
			*it != constants::SECTION_END && *it != constants::SEMICOLON) {
			it++;
			j++;
		}
		if (j - i > 0)
			token.push_back(src.substr(i, j - i));
		i = j;
	}
	return token;
}

std::vector<std::string>			Parser::ParseFile(std::string filename) {
	std::vector<std::string> file;
	std::string				lines = "";
	std::vector<std::string> parsed;
	

	file = getLineFile(filename);
	std::vector<std::string>::iterator it = file.begin();
	std::vector<std::string>::iterator ite = file.end();
	for ( ; it != ite; it++) {
		lines += StripLine(*it);
	}
	parsed = ParseSection(lines);
	return parsed;
}



