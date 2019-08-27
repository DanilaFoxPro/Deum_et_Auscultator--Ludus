#include "server.h"
#include "shared.h"
#include "network.h"
#include "string.h"


#include <memory>

void Server::checkMessages(){
	
	//select() destroys stuff, so copy master before giving it to that monster!
	fd_set copy = master;

	// See who's talking to us
	int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

	// Loop through all the (current / potential) connections 
	for (int i = 0; i < socketCount; i++)
	{
		// Makes things easy for us doing this assignment
		SOCKET sock = copy.fd_array[i];

		// Is it an inbound communication?
		if (sock == listening)
		{
			// Accept a new connection
			SOCKET client = accept(listening, nullptr, nullptr);
			
			// Add the new connection to the list of connected clients
			FD_SET(client, &master);

			// Send a welcome message to the connected client
			this->messageClient(client, "You are connected to the " + name + ".", getDataOfMessageType(message_type::personal));
			
			//Log stuff
			this->log( "New client connected, put him on socket #" + std::to_string(client) + "." );
			
			//Create userinfo and add to default room.
			{
				
				UserInfo newConnection;
				
				newConnection.socket = client;
				newConnection.username = stringh::randomName();
					
				if(newConnection.username.size()==0)
					newConnection.username = "SOCKET"+std::to_string(client);
				
				
				rooms[0]->members.emplace_back( std::shared_ptr<UserInfo>(new UserInfo(newConnection)) );
				
				//Announce new connection to already connected.
				this->messageAllClients( newConnection.username + " connected." );
				
			}
			
			
			
		}
		else // It's an inbound message
		{
			this->getMessage(sock);
		}
	}
	
}

bool Server::messageAllClients(SOCKET source, std::string message){
	
	std::shared_ptr<UserInfo> sourceInfo = this->getUserInfoFromSocket(source);
	std::string user_message = sourceInfo->username + ": " + message;
	
	MessageData msgData;
	
	for( unsigned int i = 0; i < master.fd_count; i++ ){
		
		SOCKET outSock = master.fd_array[i];
		if (outSock != listening)
		{
			
			if(outSock == source)
				msgData.type = message_type::self;
			else
				msgData.type = message_type::user;
			
			this->messageClient(outSock, user_message, msgData);
		}
	}
	
	return true;
	
}

bool Server::messageAllRoommates(SOCKET source, std::string message){
	
	// Send message to other clients
	
	std::shared_ptr<UserInfo> sourceInfo = this->getUserInfoFromSocket(source);
	std::shared_ptr<ChatRoom> source_room = this->getChatRoomFromSocket(source);
	std::string user_message = sourceInfo->username + ": " + message;
	
	MessageData msgData;
	
	if(!source_room)
		return false;
	
	//std::vector< std::shared_ptr<UserInfo> > members = ;
	
	for ( std::shared_ptr<UserInfo> current : (source_room->members) ){
		
		SOCKET outSock = current->socket;
		
		if(outSock == source)
			msgData.type = message_type::self;
		else
			msgData.type = message_type::user;
			
		this->messageClient(outSock, user_message, msgData);
		
	}
	
	return true;
	
}

bool Server::messageAllClients(std::string message){
	
	this->log( "Server Message: " + ("\""+message+"\"") );
	
	MessageData msgData;
	msgData.type = message_type::global;
	
	for (unsigned int i = 0; i < master.fd_count; i++)
	{
		SOCKET outSock = master.fd_array[i];
		if (outSock != listening)
		{
			this->messageClient(outSock, message, msgData);
		}
	}
	
	return true;
	
}

bool Server::messageClient(SOCKET client, std::string message, MessageData data){
	
	bool long_msg = message.length() > message_buffer_size;
	
	strcpy((char*)&data.checksum, (char*)&message_data_checksum);
	
	char dataToSend[message_size];
	char* messageCharPtr = (char*)message.c_str();
	
	

	memcpy( &dataToSend, messageCharPtr, (long_msg ? message_buffer_size : message.length() ) );
	dataToSend[message.length()] = '\0';
	memcpy(&dataToSend[message_size-sizeof(MessageData)], (char*)&data, sizeof(MessageData));
	
	send(client, dataToSend, message_size, 0);
	if(long_msg)
		this->messageClient(client, message.substr(message_buffer_size, message.length()-message_buffer_size), data);
	return true;
	
}

void Server::getMessage(SOCKET messager){
	
	char ReciveBuffer[message_size];
	ZeroMemory(ReciveBuffer, message_size);
	
	// Receive message
	int bytesIn = recv(messager, ReciveBuffer, message_size, 0);
	
	MessageData data;
	 	
	memcpy(&data, &ReciveBuffer[message_size-sizeof(MessageData)], sizeof(MessageData));
	
	std::string log_msg;
	
	if( memcmp( &data.checksum, message_data_checksum, 8 )==0 ){
		//Message not corrupt.
	} else {
		log_msg += "(Corrupt):";
	}
	
	log_msg += "New message on sock " + std::to_string(messager) + ": " + std::string(ReciveBuffer);
	
	this->log( log_msg );
	if (bytesIn <= 0)
	{
		
		this->dropUser( messager );
		
	}
	else
	{
		
		// Check to see if it's a command. /quit kills the server
		if(ReciveBuffer[0] == '/') {
			
			this->interpretCommand( this->getUserInfoFromSocket( messager ), ReciveBuffer);
			return;
			
		}

		this->messageAllRoommates(messager, std::string(ReciveBuffer));
	}
	
}
