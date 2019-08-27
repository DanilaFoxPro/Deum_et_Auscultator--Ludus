#ifndef __KEYPRESS_H_
#define __KEYPRESS_H_

#include <vector>

//Stolen from maze 2.0.

const int VK_UNKNOWN = -1;

bool keyPressed(int key);
void pauseUntil(int key);

std::vector<int> keyPressedMulti(std::vector<int> keys);
std::vector<int> keyPressedMulti(const int keys[], unsigned int key_count);

int keyPressedMultiOne(std::vector<int> keys, bool sticky = true);

#endif
