#ifndef __NUMBERS_H_
#define __NUMBERS_H_

#include <vector>
#include <array>

std::vector<unsigned int> range(unsigned int begin, unsigned int end);
std::vector<int> range(int begin, int end);
unsigned int power(unsigned int base, unsigned int power);

//Category 1

template<typename T>
bool inRange(T thing, T range_begin, T range_end){
	return ((thing >= range_begin) && (thing <= range_end));
}

template<typename T>
bool oneOfThose(T thing, std::vector<T> those){
	
	for(T one_of_those_those : those){
		if(thing == one_of_those_those)
			return true;
	}
	
	return false;
	
}

//Category 2

template<typename T>
T map(T input, std::vector<std::array<int, 2>> map_to, T fail_code){
	
	for(unsigned int i = 0; i < map_to.size(); i++){
		if(map_to[i][0] == input)
			return map_to[i][1];
	}
	
	return fail_code;
	
}

#endif
