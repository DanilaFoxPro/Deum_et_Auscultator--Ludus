#include "numbers.h"

std::vector<unsigned int> range(unsigned int begin, unsigned int end){
	std::vector<unsigned int> output;
	for(unsigned int i = begin; i <= end; i++)
		output.push_back(i);
	return output;
}

std::vector<int> range(int begin, int end){
	std::vector<int> output;
	for(int i = begin; i <= end; i++)
		output.push_back(i);
	return output;
}

unsigned int power(unsigned int base, unsigned int power){
	if(power==0)
		return 1;
	
	unsigned int output = base;
	while(power>1){
		output *= base;
		power--;
	}
	return output;
}
