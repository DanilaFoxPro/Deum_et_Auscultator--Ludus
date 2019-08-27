#ifndef _COLOR_H__
#define _COLOR_H__

#include <string>
#include <vector>

//IF WINDOWS
#if (defined (_WIN32) || defined (_WIN64))
    #include <windows.h>
#endif

struct colored_string{
	std::string str;
	unsigned char color;
};

//Change console color.
void color(unsigned char color);

void printColoredChunk(std::vector<colored_string> color_strings);

#endif
