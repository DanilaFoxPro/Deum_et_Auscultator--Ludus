#include "keypress.h"

#include <iostream>

//IF WINDOWS
#if (defined (_WIN32) || defined (_WIN64))
    #include <windows.h>
#endif

bool keyPressed(int key){
	
	return GetKeyState(key) & 0x8000;
	
}

void pauseUntil(int key){
	for(;;){
		if(keyPressed(key)){
			while(keyPressed(key));
			return;
		}
	}
}

std::vector<int> keyPressedMulti(std::vector<int> keys){
	
	if(GetConsoleWindow() != GetForegroundWindow())
		return {};
	
	std::vector<int> output;
	unsigned int keyCount = keys.size();
	output.reserve(keyCount);
	
	for(unsigned int i = 0; i < keyCount; i++)
		if(keyPressed(keys[i]))
			output.push_back(keys[i]);
	
	return output;
	
}

std::vector<int> keyPressedMulti(const int keys[], unsigned int key_count){
	
	std::vector<int> keys_;
	keys_.reserve(key_count);
	
	for(unsigned int i = 0; i < key_count; i++)
		keys_.push_back(keys[i]);
	
	return keyPressedMulti(keys_);
	
}

int keyPressedMultiOne(std::vector<int> keys, bool sticky){
	
	if(GetConsoleWindow() != GetForegroundWindow())
		return -1;
	
	unsigned int keyCount = keys.size();
	for(unsigned int i = 0; i < keyCount; i++)
		if(keyPressed(keys[i])){
			
			if(sticky)
				while(keyPressed(keys[i]));
				
			return keys[i];
		}
			
	
	return -1;
	
}
