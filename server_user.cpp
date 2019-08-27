#include "server.h"
#include "shared.h"

std::shared_ptr<UserInfo> Server::getUserInfoFromSocket(SOCKET User){
	
	for( std::shared_ptr<ChatRoom> room : rooms ){
		
		for( std::shared_ptr<UserInfo> current : room->members ){
			
			if(current->socket == User)
				return current;
			
		}
	}
	
	return nullptr;
	
}

std::shared_ptr<ChatRoom> Server::getChatRoomFromSocket(SOCKET User){
	
	for( std::shared_ptr<ChatRoom> room : rooms ){
		
		for( std::shared_ptr<UserInfo> current : room->members ){
			
			if(current->socket == User)
				return room;
			
		}
	}
	
	return nullptr;
	
}

std::shared_ptr<ChatRoom> Server::getChatRoomFromUserInfo(UserInfo userInf){
	
	for( std::shared_ptr<ChatRoom> room : rooms ){
		
		for( std::shared_ptr<UserInfo> current : room->members ){
			
			if(*current == userInf)
				return room;
			
		}
	}
	
	return nullptr;
	
}

unsigned int Server::getRoomOrMemberIDFromUserInfo(UserInfo userInf, bool Room){
	
	for( unsigned int i = 0; i < rooms.size(); i++ ){
		
		for( unsigned int j = 0; j < rooms[i]->members.size(); j++ ){
			
			if(*(rooms[i]->members[j])==userInf)
				return (Room ? i : j);
			
		}
		
	}
	
	return ui_invalid_id;
	
}

unsigned int Server::getRoomIDFromUserInfo(UserInfo userInf){
	
	return getRoomOrMemberIDFromUserInfo(userInf, true);
	
}

unsigned int Server::getMemberIDFromUserInfo(UserInfo userInf){
	
	return getRoomOrMemberIDFromUserInfo(userInf, false);
	
}

bool Server::moveUserToRoom(UserInfo userInf, unsigned int room_id){
	
	unsigned int source_user_id = this->getMemberIDFromUserInfo(userInf);
	unsigned int source_room_id = this->getRoomIDFromUserInfo(userInf);
	
	if( (source_room_id == ui_invalid_id) or (source_user_id == ui_invalid_id) )
		return false;
	
	this->rooms[room_id]->members.emplace_back( new UserInfo( userInf ) );
	this->rooms[source_room_id]->members.erase(this->rooms[source_room_id]->members.begin()+source_user_id);
	
	return true;
	
}

void Server::dropUser(SOCKET user){
	
	// Drop the client
	closesocket(user);
	FD_CLR(user, &master);
	
	std::shared_ptr<UserInfo> userInfo = this->getUserInfoFromSocket( user );
	
	this->log( "Dropped client on sock " + std::to_string(user) + "." );
	this->messageAllClients( userInfo->username + " has disconnected." );
	
	//Remove client name from memory
	std::shared_ptr<ChatRoom> room = this->getChatRoomFromSocket(user);
	
	if(!room)
		return;
	
	unsigned int usercount = room->members.size();
	if(1<usercount){
		for(std::shared_ptr<UserInfo> current : room->members){
			if(current->socket==user){
				current = room->members[usercount-1];
			}
		}
	}
	
	room->members.pop_back();
	
}
