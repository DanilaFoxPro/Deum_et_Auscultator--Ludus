#include "color.h"

#include <iostream>

void color(unsigned char color){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	return;
}

void printColoredChunk(std::vector<colored_string> color_strings){
	for(colored_string color_string : color_strings){
		color(color_string.color);
		std::cout << color_string.str;
	}
}
