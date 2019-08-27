#include "chat_ui.h"

#include "..\input\keypress.h"
#include "..\utility\numbers.h"

//Vector hack
std::vector<int> operator+(const std::vector<int>& A, const std::vector<int>& B){
	
	std::vector<int> comb;
	comb.reserve(A.size() + B.size());
	comb.insert( comb.end(), A.begin(), A.end() );
	comb.insert( comb.end(), B.begin(), B.end() );
	
	return comb;
	
}

void ClientChat::checkUserInput(){
	
	std::vector<int> additionalKeys = {VK_BACK, VK_RETURN, VK_SPACE, VK_OEM_MINUS, VK_OEM_PLUS, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2, VK_OEM_7};
	
	int key = keyPressedMultiOne(range(0x30, 0x39) + range(0x41, 0x5A) + additionalKeys, false);
	
	//std::cout << key << std::endl;
	
	if(readyForInput){
		
		//Return, Backspace
		switch(key){
			
			case -1:{
				return;
				break;
			}
			
			case VK_BACK:{
				
				if(userInput.size())
					userInput.pop_back();
				
				readyForInput = false;
				should_redraw = true;
				
				break;
			}
			
			case VK_RETURN:{
				
				readyForInput = false;
				should_redraw = true;
				
				this->sendUserInput();
				
				break;
			}
			
			default:{
				
				//SHIFT
				if(keyPressed(VK_SHIFT)){
					
					//Digit (0-9)
					if( inRange(key, 48, 57) ){
						this->addUserInput(map(key, {{48, 41}, {49, 33}, {50, 64}, {51, 35}, {52, 36}, {53, 37}, {54, 94}, {55, 38}, {56, 42}, {57, 40}}, 0));
					}
					//Letters
					else if( inRange(key, 65, 90) ) {
						this->addUserInput(key);
					}
					//Minus, Plus, Comma, Period, Slash.
					else if( oneOfThose(key, {VK_OEM_MINUS, VK_OEM_PLUS, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2, VK_OEM_7}) ){
						this->addUserInput( map(key, {{VK_OEM_MINUS, '_'}, {VK_OEM_PLUS, '+'}, {VK_OEM_COMMA, '<'}, {VK_OEM_PERIOD, '>'}, {VK_OEM_2, '?'}, {VK_OEM_7, '"'}}, 0) );
					}
					//Other
					else {
						//Signal that the key is not supported.
						this->addUserInput('?');
					}
					
				} else {
					
					//Letters
					if( inRange(key, 65, 90) ){
						//Virual keys for letters correspond with capital letters.
						this->addUserInput(key+32);
					}
					//Minus, Plus, Comma, Period, Slash.
					else if( oneOfThose(key, {VK_OEM_MINUS, VK_OEM_PLUS, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2, VK_OEM_7}) ){
						this->addUserInput( map(key, {{VK_OEM_MINUS, '-'}, {VK_OEM_PLUS, '='}, {VK_OEM_COMMA, ','}, {VK_OEM_PERIOD, '.'}, {VK_OEM_2, '/'}, {VK_OEM_7, '\''}}, 0) );
					}
					//Other
					else {
						this->addUserInput(key);
					}
					
				}
				
				readyForInput = false;
				should_redraw= true;
				
				break;
			}
		
		}
		
	} else {
		
		if(key == -1){
			readyForInput = true;
			return;
		}
		
	}
	
}

void ClientChat::addUserInput(char append){
	
	if(userInput.size() < user_input_max)
		userInput.push_back(append);
	
}
