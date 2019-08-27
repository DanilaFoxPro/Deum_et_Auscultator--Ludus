#ifndef __SERVER_COMMANDS_H_
#define __SERVER_COMMANDS_H_

#include <vector>
#include <string>
#include <memory>

//#include "server.h"

class Server;
struct UserInfo;

namespace concom {
	
	struct Parameters {
		
		Server* server;
		std::shared_ptr<UserInfo> userinfo;
		std::vector<std::string> arguments;
		
	};
	
	namespace server{
		
		void help(Parameters parameters);
		
		void quit(Parameters parameters);
		
		void setname(Parameters parameters);
		void room(Parameters parameters);
		
	}
	
}

#endif
