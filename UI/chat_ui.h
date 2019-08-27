#ifndef __CHAT_UI_H_
#define __CHAT_UI_H_

#include <iostream>
#include <vector>
#include <WS2tcpip.h>

#include "..\shared.h"


const unsigned char COLOR_NONE			= char(7);
const unsigned char COLOR_GLOBAL		= char(13);
const unsigned char COLOR_PERSONAL		= char(14);
const unsigned char COLOR_USER			= char(2);
const unsigned char COLOR_SELF			= char(10);


enum state{
	disconnect,
	error,
};

struct log_line {
	
	std::string message;
	int type;
	
};

class ClientChat{
	private:
		SOCKET ConnectSocket;
		bool connected;
		
		//Inbound
		bool updated;
		log_line msg;
		
		//User
		std::string userInput;
		
		//Log
		std::vector<log_line> log;
		
		//Graphic
		bool should_redraw;
		
		bool readyForInput;
		
	public:
		
		void setConnectSocket(SOCKET ConnectSocket_);
		
		
		void updateInbound();
		
		void checkUserInput();
		void addUserInput(char append);
		void sendUserInput();
		
		state BringChatUI();
		
		void addToLog(log_line logl);
		void redraw();
		
		
		ClientChat(){
			updated = false;
			msg = {"000", char(17)};
			
			userInput.reserve(user_input_max);
			log.resize(client_log_size);
		}
};

#endif
