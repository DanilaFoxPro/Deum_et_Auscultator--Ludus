#ifndef __SHARED_H_
#define __SHARED_H_

#include <string>

#define DISCONNECT_MSG		"Please disconnect. -Love, server."
#define CHECKSUM_SIZE		8

//Structs

struct MessageData {
	
	int type = 0;
	char checksum[CHECKSUM_SIZE];
	
};

namespace message_type {
	
	enum {
		
		none,
		global,
		personal,
		user,
		self,
		
	};
	
}


//Constants!!!
const unsigned int message_buffer_size	= 128;
const unsigned int message_size			= message_buffer_size + sizeof(MessageData);

const unsigned int user_input_max		= 92;
const unsigned int user_name_max		= 16;

const unsigned int client_log_size		= 20;

const std::string message_check_postfix = "//__//__//__//";
const char message_data_checksum[CHECKSUM_SIZE]		= {'%', '%', '%', '%', '%', '%', '%', '%'};

const unsigned int server_max_rooms		= 22;

const unsigned int ui_invalid_id		= (unsigned int)(-1);

#endif
