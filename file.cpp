#include "file.h"

#include <fstream>

namespace file {
	
	std::vector<std::string> getLines(std::string filename){
		
		std::fstream input;
		input.open(filename);
		
		if(!input.is_open())
			return {};
		
		std::string line;
		std::vector<std::string> output;
		while(getline(input, line))
			output.push_back(line);
		
		return output;
		
	}
	
}
