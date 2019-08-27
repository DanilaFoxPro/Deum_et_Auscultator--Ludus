#include "server_commands.h"

#include "string.h"
#include "network.h"
#include "chat_types.h"

#include "server.h"

//Macros
#define invalidSyntax()		parameters.server->messageClient(parameters.userinfo->socket, "Invalid syntax.", getDataOfMessageType(message_type::personal))
#define clientmsg(x)		parameters.server->messageClient(parameters.userinfo->socket, x, getDataOfMessageType(message_type::personal))

namespace concom {
	
	namespace server{
		
		void help(Parameters parameters){
			
			parameters.server->log("Someone tried to use help. Poor soul.");
			clientmsg("Sorry, nothing yet.");
			return;
			
		}
		
		void quit(Parameters parameters){
			
			parameters.server->log("Server: Recived \"/quit\" command.");
			parameters.server->closeServer();
			return;
			
		}
		
		void setname(Parameters parameters){
			
			std::string newName = stringh::unquote( parameters.arguments[0] );
			
			if(newName.size()>user_name_max){
				clientmsg("Max username length: " + std::to_string( user_name_max ) + ", your input: " + std::to_string( newName.size() ) + ".\nRequest denied.");
				return;
			}
			
			parameters.userinfo->username = newName;
			
			parameters.server->log(std::string("Set sock ") + std::to_string(parameters.userinfo->socket) + std::string("'s username to ") + parameters.arguments[0] + ".");
			clientmsg("Name changed.");
			
			return;
			
		}
		
		void room(Parameters parameters){
			
			switch(parameters.arguments.size()){
				
				case 1:{
					
					if(parameters.arguments[0]=="list"){
						
						std::vector< std::shared_ptr<ChatRoom> > chatroomz = parameters.server->getChatRooms();
						
						for(unsigned int i = 0; i < chatroomz.size(); i++){
							
							clientmsg( stringh::padTo(std::to_string(i), 3) + " - " + chatroomz[i]->name );
							
						}
						
						clientmsg("End.");
						return;
						
					}
					
					break;
				}
				
				case 2:{
					
					if(parameters.arguments[0] == "create"){
						
						std::string room_name = stringh::unquote(parameters.arguments[1]);
						
						if(room_name.size()==0){
							invalidSyntax();
							return;
						}
						
						std::vector< std::shared_ptr<ChatRoom> > chatroomz = parameters.server->getChatRooms();
						
						//Check if name is original.
						for( std::shared_ptr<ChatRoom> cur : chatroomz ){
							
							if(cur->name == room_name){
								
								clientmsg("Chat room name already taken.");
								return;
								
							}
							
						}
						
						//Check if limit is reached.
						if(chatroomz.size()==server_max_rooms){
							clientmsg("Chat room limit reached. No more.");
							return;
						} else if(chatroomz.size() > server_max_rooms){
							clientmsg("Ahem. Chat room limit is surpassed, so I guess you are free to create another one. Unless unexpected error occurs.");
						}
						
						ChatRoom new_room;
						new_room.name = room_name;
						new_room.type = room_type::chat;
						
						unsigned int new_room_id = parameters.server->addChatRoom(new_room);
						
						if( new_room_id == ui_invalid_id ){
							
							clientmsg("Unexpected error.");
							return;
							
						}
						
						parameters.server->log("Socket" + std::to_string((int)parameters.userinfo->socket) + std::string("(") + parameters.userinfo->username + ") created a chat room named '" + room_name + "', id " + std::to_string(new_room_id) + ".");
						clientmsg("Room successfully created.");
						
						return;
						
					} else if (parameters.arguments[0] == "remove"){
						
						std::string room_id_str = stringh::unquote(parameters.arguments[1]);
						
						//Check if NaI.
						if(!stringh::isInteger(room_id_str)){
							clientmsg("Can't read room ID.");
							return;
						}
						
						std::vector< std::shared_ptr<ChatRoom> > chatroomz = parameters.server->getChatRooms();
						unsigned int room_id = stoul(room_id_str);
						
						if(room_id==0 || room_id>=chatroomz.size()){
							clientmsg("Room ID out of range.");
							return;
						}
						
						//Move all members to the default room.
						parameters.server->transferMembers( chatroomz[room_id], chatroomz[0] );
						/*chatroomz[0]->members.reserve(chatroomz[0]->members.size() + chatroomz[room_id]->members.size());
						for(std::shared_ptr<UserInfo> member : chatroomz[room_id]->members)
							chatroomz[0]->members.emplace_back(member);*/
						
						//
						if(!parameters.server->removeChatRoom( room_id )){
							
							clientmsg("Unexpected error occured.");
							return;
							
						}
						
						clientmsg("Successefully removed room ID " + std::to_string(room_id) + " ('" + chatroomz[room_id]->name + "').");
						parameters.server->log( "Socket" + std::to_string((int)parameters.userinfo->socket) + std::string("(") + parameters.userinfo->username + ") removed a room '" + chatroomz[room_id]->name + "', id " + std::to_string(room_id) + "." );
						return;
						
						
					} else if (parameters.arguments[0] == "join"){
						
						std::string room_id_str = stringh::unquote(parameters.arguments[1]);
						
						//Check if NaI.
						if(!stringh::isInteger(room_id_str)){
							clientmsg("Can't read room ID.");
							return;
						}
						
						std::vector< std::shared_ptr<ChatRoom> > chatroomz = parameters.server->getChatRooms();
						unsigned int room_id = std::stoul(room_id_str);
						
						if(room_id>=chatroomz.size()){
							clientmsg("Room ID out of range.");
							return;
						}
						
						unsigned int target_room_id = parameters.server->getRoomIDFromUserInfo( *parameters.userinfo );
						
						if( room_id == target_room_id ){
							clientmsg("You're already in that room.");
							return;
						}
						
						if(!parameters.server->moveUserToRoom( *parameters.userinfo, room_id )){
							clientmsg("Unexpected error.");
							return;
						}
						
						/*unsigned int member_id = parameters.server->getMemberIDFromUserInfo( *parameters.userinfo );
						chatroomz[target_room_id]->members.emplace_back( new UserInfo( *parameters.userinfo ) );
						chatroomz[room_id]->members.erase( chatroomz[room_id]->members.begin()+member_id );*/
						
						/* TODO (DFP#1#): Do UserInfo moving on the server. Also add 
						                  ChatRoom welcome messages. */
						
						clientmsg("Successefully moved.");
						return;
						
					}
					
					break;
				}
				
				default:{
					parameters.server->log_error("Error: This shouldn't ever happen.");
					break;
				}
				
			}
			
			invalidSyntax();
			return;
			
		}
		
	}
	
}
