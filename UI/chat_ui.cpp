#include "chat_ui.h"
#include "..\shared.h"

#include "..\string.h"
#include "..\symbols.h"

#include "..\utility\numbers.h"
#include "..\input\keypress.h"
#include "..\color.h"

#include <thread>
#include <atomic>

void setTypeColor(int type){
	
	switch(type){
		
		case message_type::global:{
			color(COLOR_GLOBAL);
			break;
		}
		
		case message_type::personal:{
			color(COLOR_PERSONAL);
			break;
		}
		
		case message_type::user:{
			color(COLOR_USER);
			break;
		}
		
		case message_type::self:{
			color(COLOR_SELF);
			break;
		}
		
		default:{
			color(COLOR_NONE);
			break;
		}
		
	}
	
	return;
	
}

void ClientChat::setConnectSocket(SOCKET ConnectSocket_){
	
	ConnectSocket = ConnectSocket_;
	
}

void ClientChat::updateInbound(){
	
	std::cout << "UPDATEINBOUND\n";
	
	char ReciveBuffer[message_size];
    
    int iResult;
    
    std::string ReciveBufferStr;
    
    for(;;){
    	
    	iResult = recv(ConnectSocket, ReciveBuffer, message_size, 0);
	    
	  	ReciveBufferStr = std::string(ReciveBuffer);
	  		
	 	MessageData data;
	 	
	 	memcpy(&data, &ReciveBuffer[message_size-sizeof(MessageData)], sizeof(MessageData));
	 		  		
	 	if( memcmp( &data.checksum, message_data_checksum, 8 )==0 ){
	 		
	 		//Message is alright.
	 		
		} else {
			
			ReciveBufferStr += (" [Corrupt]" + std::string(data.checksum));
			for( unsigned int i = 0; i < sizeof(data.checksum); i++ ){
				ReciveBufferStr += ((data.checksum[i] == '\0') ? 'Z' : data.checksum[i]);
			}
			
		}
	  	
	  	msg.message = ReciveBufferStr;
	  	msg.type = data.type;
	    
	    if((msg.message == DISCONNECT_MSG) or (iResult == SOCKET_ERROR)){
	    	updated = false;
	    	connected = false;
	    	
	    	return;
		} else {
			
			updated = true;
			
		}
		
		while(updated); //Wait for the client to handle the message before waiting for new one.
	    
	}
    
}

state ClientChat::BringChatUI(){
	
	connected = true;
	std::thread inbound(&ClientChat::updateInbound, this);
    
    //Connection loop.
    
    do{
    	
    	//std::cout << "CONLOOP\n";
    	
		checkUserInput();
		
		if(updated){
			
			this->addToLog(msg);
			
			should_redraw = true;
			updated = false;
			
		}
		
		if(should_redraw)
			redraw();
		
	}while(msg.message.size() and connected);
	
	system("cls");
	
	std::cout << "Log Dump:" << std::endl;
	
	for(log_line line : log)
		std::cout << line.message << std::endl;
		
	std::cout << "Connection terminated.\n";
	
	inbound.join();
	
	std::cout << "Press any key...";
	system("pause>nul");
	std::cout << std::endl;
	
	return disconnect;
	
}

void ClientChat::addToLog(log_line logl){
	
	std::cout << "ADDTOLOG\n";
	
	//Shift everything up by 1
	for(unsigned int i = 0; i < log.size()-1; i++)
		log[i] = log[i+1];
	
	//Display new msg on the available slot
	log[log.size()-1] = logl;
	
}

void ClientChat::redraw(){
	
	system("cls");
	const unsigned int message_padding = 2;
	color(COLOR_NONE);
	
	//Chat top border
	std::cout << border::double_corner_top_left << std::string(message_buffer_size+message_padding, border::double_horizontal) << border::double_corner_top_right << std::endl;
	
	//Chat messages
	std::vector<std::string> sublines;
	for(log_line line : log){
		
		sublines = stringh::splitString(line.message, '\n');
		
		for(std::string subline : sublines){
			color(COLOR_NONE);
			std::cout << border::double_vertical;
			setTypeColor(line.type);
			std::cout << std::string(message_padding/2, ' ') << stringh::padTo(subline, message_buffer_size) << std::string(message_padding/2+message_padding%2, ' ');
			color(COLOR_NONE);
			std::cout << border::double_vertical << std::endl;
		}
		
	}
	
	//Chat bottom border // User Input top border
	std::cout << border::double_junction_left << std::string(message_buffer_size+message_padding, border::double_horizontal) << border::double_junction_right << std::endl;
	
	//User Input
	std::cout << border::double_vertical << std::string(message_padding/2, ' ') << stringh::padTo( "Message:" + userInput, message_buffer_size) << std::string(message_padding/2+message_padding%2, ' ') << border::double_vertical << std::endl;
	
	//User Input bottom border
	std::cout << border::double_corner_bottom_left << std::string(message_buffer_size+message_padding, border::double_horizontal) << border::double_corner_bottom_right << std::endl;
	
	should_redraw = false;
	
}

void ClientChat::sendUserInput(){
	
	MessageData data;
	std::string safeUserInput = userInput;
	
	data.type = message_type::user;
	memcpy((char*)&data.checksum, (char*)&message_data_checksum, CHECKSUM_SIZE);
	
	char dataToSend[message_size];
	
	memcpy(&dataToSend, safeUserInput.c_str(), message_size-sizeof(MessageData));
	dataToSend[safeUserInput.length()] = '\0';
	memcpy(&dataToSend[message_size-sizeof(MessageData)], (char*)&data, sizeof(MessageData));
	
	send( ConnectSocket, dataToSend, message_size, 0 );
	
	userInput.clear();
	
}
