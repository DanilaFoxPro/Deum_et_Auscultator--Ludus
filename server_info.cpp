#include "server.h"

#include <iostream>

void Server::log(std::string msg){
	
	std::cout << msg << std::endl;
	return;
	
}

void Server::log_error(std::string msg){
	
	std::cerr << msg << std::endl;
	return;
	
}

std::vector< std::shared_ptr<fcommand> > Server::getCommands(){
	
	return commands;
	
}

std::vector< std::shared_ptr<ChatRoom> > Server::getChatRooms(){
	
	return rooms;
	
}
