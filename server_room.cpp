#include "server.h"
#include "shared.h"

bool Server::removeChatRoom(unsigned int id){
	
	if(id == 0 || id >= rooms.size())
		return false;
	
	rooms.erase(rooms.begin()+id);
	return true;
	
}

unsigned int Server::addChatRoom(ChatRoom room){
	
	if( rooms.size() >= server_max_rooms )
		return ui_invalid_id;
	
	rooms.emplace_back( new ChatRoom( room ) );
	return rooms.size()-1;
	
}

bool Server::transferMembers( std::shared_ptr<ChatRoom> source, std::shared_ptr<ChatRoom> destination ){
	
	//Check valid.
	if(!(source and destination))
		return false;
	
	destination->members.reserve( source->members.size() + destination->members.size() );
	for( std::shared_ptr<UserInfo> member : source->members )
		destination->members.push_back( member );
	
	source->members.clear();
	
	return true;
	
}
