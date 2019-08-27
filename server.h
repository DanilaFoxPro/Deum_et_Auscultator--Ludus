#ifndef __SERVER_H_
#define __SERVER_H_

#include <WS2tcpip.h>

#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include "shared.h"

//Games
#include "roles.h"

//UserInfo

struct UserInfo{
	
	std::string username;
	SOCKET socket;
	
	int chat_role = role::chat::none;
	
};

bool operator==(const UserInfo& x, const UserInfo& y);
bool operator!=(const UserInfo& x, const UserInfo& y);

//Room

struct ChatRoom{
	
	std::string name;
	int type;
	std::vector< std::shared_ptr<UserInfo> > members;
	
};

bool operator==(const ChatRoom& x, const ChatRoom& y);
bool operator!=(const ChatRoom& x, const ChatRoom& y);

//Command

class Server;
#include "server_commands.h"

struct fcommand{
	
	std::string command;
	void (*pointer)(concom::Parameters);
	
	std::string short_description;
	
	unsigned int arguments_min;
	unsigned int arguments_max;
	
};


//Server

class Server {
	private:
		//Server Stuff.
		std::string name;
		unsigned int port;
		bool running;
		
		//Socket Stuff.
		SOCKET listening;
		fd_set master;
		
		//User Info Stuff.
		std::vector< std::shared_ptr<ChatRoom> > rooms;
	
		//Commands.
		std::vector< std::shared_ptr<fcommand> > commands;
		
		
	public:
		void runServer();
		void closeServer();
		
		void setPort(unsigned int port_);
		void setName(std::string name_);
		
		void log(std::string msg);
		void log_error(std::string msg);
		
		//Message
		bool messageAllClients(SOCKET source, std::string message);
		bool messageAllRoommates(SOCKET source, std::string message);
		bool messageAllClients(std::string message);
		bool messageClient(SOCKET client, std::string message, MessageData msgData);
		
		//User
		std::shared_ptr<UserInfo> getUserInfoFromSocket(SOCKET User);
		std::shared_ptr<ChatRoom> getChatRoomFromSocket(SOCKET User);
		std::shared_ptr<ChatRoom> getChatRoomFromUserInfo(UserInfo userInf);
		unsigned int getRoomIDFromUserInfo(UserInfo userInf);
		unsigned int getMemberIDFromUserInfo(UserInfo userInf);
		
		void dropUser(SOCKET user);
		
		bool moveUserToRoom(UserInfo userInf, unsigned int room_id);
		
		//Room
		bool removeChatRoom(unsigned int id);
		unsigned int addChatRoom(ChatRoom room);
		
		bool transferMembers( std::shared_ptr<ChatRoom> source, std::shared_ptr<ChatRoom> destination );
		
		//External Access
		std::vector< std::shared_ptr<ChatRoom> > getChatRooms();
		std::vector< std::shared_ptr<fcommand> > getCommands();
		
	private:
		void tick();
		
		//User
		unsigned int getRoomOrMemberIDFromUserInfo(UserInfo userInf, bool Room);
		
		//Message
		void checkMessages();
		void getMessage(SOCKET messager);
		
		//Command
		void registerCommands();
		void interpretCommand(std::shared_ptr<UserInfo> messager, std::string message);
		
		void registerCommand(fcommand command);
		
		//Info
		bool areAllReady();
};

#endif
