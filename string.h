#ifndef __STRING_H_
#define __STRING_H_

#include <vector>
#include <string>

namespace stringh {
	
	std::vector<std::string> splitString(std::string input, char spliton = ' ');
	std::vector<std::string> parseString(std::string input);
	
	std::string unquote(std::string input);
	std::string padTo(std::string input, unsigned int length);
	
	bool isInteger(std::string input);
	bool isFloat(std::string input);
	
	std::string randomName();
	
}

#endif
