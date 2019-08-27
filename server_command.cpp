#include "server.h"
#include "shared.h"
#include "network.h"

#include <string>

#include "string.h"
#include "server_commands.h"

void Server::registerCommands(){
		
	this->registerCommand({"help", &concom::server::help, "List all commands with short descriptions.", 0, 0});
	
	this->registerCommand({"quit", &concom::server::quit, "Close the server.", 0, 0});
	
	this->registerCommand({"setname", &concom::server::setname, "Set your name.", 1, 1});
	this->registerCommand({"room", &concom::server::room, "Manage server rooms.", 1, 2});
	
	
}

void Server::registerCommand(fcommand command){
	
	if(command.pointer == nullptr)
		return;
	
	unsigned int position = (unsigned int)(-1);
	
	for(unsigned int i = 0; i < commands.size(); i++){
		
		if(commands[i]->command == command.command){
			position = i;
		}
		
	}
	
	if( position == (unsigned int)(-1) ){
		
		commands.emplace_back( std::shared_ptr<fcommand>(new fcommand(command)) );
		
	} else {
		
		commands[position] = std::shared_ptr<fcommand>(new fcommand(command));
		
	}
	
	return;
	
}

void Server::interpretCommand(std::shared_ptr<UserInfo> messager, std::string message){
	
	std::vector<std::string> parsed = stringh::parseString( message );
	//unsigned int argument_count = ( (parsed.size()==0) ? 0 : parsed.size()-1 );
	
	if(parsed[0].size() < 2)
		return;
	
	std::string command = parsed[0].substr(1);
	parsed.erase(parsed.begin());
	
	for(std::shared_ptr<fcommand> current : commands){
		
		if(current->command == command){
			
			if( (parsed.size()<=current->arguments_max) && (parsed.size()>=current->arguments_min) ){
				
				current->pointer({this, messager, parsed});
				
			} else {
				
				this->messageClient( messager->socket, ("Parsed okay, unacceptableeeeeeee number of arguments, min:" + std::to_string(current->arguments_min) + " max:" + std::to_string(current->arguments_max) + "."), getDataOfMessageType(message_type::personal) );
				
			}
			
			
			return;
			
		}
		
	}

	// Unknown command
	this->messageClient( messager->socket, "Invalid command, I checked.", getDataOfMessageType(message_type::personal) );
	return;
	
}
