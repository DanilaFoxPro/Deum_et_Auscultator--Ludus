#include <WS2tcpip.h>
#include <string>

#include <vector>

#include "server.h"
#include "shared.h"
#include "chat_types.h"

//UserInfo

bool operator==(const UserInfo& x, const UserInfo& y){
	return std::tie(x.socket, x.username) == std::tie(y.socket, y.username);
}

bool operator!=(const UserInfo& x, const UserInfo& y){
	return !(x==y);
}

//ChatRoom

bool operator==(const ChatRoom& x, const ChatRoom& y){
	return std::tie(x.name, x.type, x.members) == std::tie(y.name, y.type, y.members);
}

bool operator!=(const ChatRoom& x, const ChatRoom& y){
	return !(x==y);
}

//Server

void Server::setName( std::string name_ ){
	
	name = name_;
	
}

void Server::setPort(unsigned int port_){
	
	port = port_;
	
}


void Server::tick(){
	
	checkMessages();
	
}


void Server::runServer (){
	
	auto bindIPandPortToListeningSocket = [&]{
		
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 
		
		bind(listening, (sockaddr*)&hint, sizeof(hint));
		
	};
	
	this->log("Starting server...");
	system("title Server");
	
	// Initialze winsock
	
	{
		WSADATA WindowsSocketData;
		WORD version = MAKEWORD(2, 2);
		
		int WindowsSocketOK = WSAStartup(version, &WindowsSocketData);
		
		if( WindowsSocketOK != 0 ){
			this->log_error("Couldn't initialize winsock! [Fatal]");
			return;
		}
	}
	
	
	// Create a socket
	{
		
		listening = socket(AF_INET, SOCK_STREAM, 0);
	
		if(listening == INVALID_SOCKET){
			this->log_error("Couldn't create a socket! [Fatal]");
			return;
		}
		
	}
	

	// Bind the ip address and port to a socket
	bindIPandPortToListeningSocket();

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Create the master file descriptor set and zero it
	FD_ZERO(&master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(listening, &master);
	
	//Add default chat room.
	this->log("Creating default chat room...");
	{
		ChatRoom defaultRoom;
		
		defaultRoom.type = room_type::chat;
		defaultRoom.name = "Default Chat Room";
		
		rooms.emplace_back( new ChatRoom(defaultRoom) );
	}
	
	//Register Commands.
	this->log("Registering commands...");
	this->registerCommands();
	
	//Tell the user that server is running
	this->log("All done. Server now running on port " + std::to_string(port) + ".");
	
	//Run loop.
	running = true;
	while (running)
	{
		this->tick();
	}
	
	this->closeServer();
	
}

void Server::closeServer(){
	
	if(running == false)
		return;
	
	this->log( "Shutting down the server..." );
	
	//...
	running = false;
	
	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(listening, &master);
	closesocket(listening);

	while (master.fd_count > 0)
	{
		SOCKET sock = master.fd_array[0];
		
		//Be nice and log the action.
		std::shared_ptr<UserInfo> currentSockInfo = this->getUserInfoFromSocket( sock );
		this->log( "Sending disconnect message to: " + std::to_string(currentSockInfo->socket) );
		
		//Send disconnect message.
		MessageData msgData;
		msgData.type = message_type::personal;
		this->messageClient( currentSockInfo->socket, DISCONNECT_MSG, msgData);

		//Remove socket from the master file list and close it.
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
	
	//Be nice and log.
	this->log( "Server successefully shut down." );
	
}
