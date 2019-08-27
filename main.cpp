//#pragma comment (lib, "ws2_32.lib")

#include <iostream>
#include <vector>
#include <string.h>
#include <cstdlib> // rand
#include <time.h> // time

#include "server.h"
#include "client.h"

#include "string.h"

int main(){
	
	srand( time(nullptr) );
	
	std::string input;
	unsigned int input_size;
	std::vector<std::string> words;
	
	system("mode 650");//Make the console big so it scales on win 7.
	
	for(;;){
		system("title Awaiting command");
		std::cout << "user:";
		
		getline(std::cin, input);
		
		if(input == "1")
			input = "run server 25565";
		
		if(input == "2")
			input = "run client localhost 25565";
		
		words = stringh::splitString(input);
		input_size = words.size();
		
		if(input_size){
			if(words[0]=="run"){
					if(input_size==3&&words[1]=="server"){
					Server DaServer;
					
					DaServer.setName("Wrong Server");
					DaServer.setPort( std::stoul(words[2]) );
					DaServer.runServer();
					
				} else if(input_size==4&&words[1]=="client"){
					char* ip = new char[words[2].size()+1];
					char* port = new char[words[3].size()+1];
					strcpy(ip, words[2].c_str());
					strcpy(port, words[3].c_str());
					runClient(ip, port);
				}
			} else if(words[0]=="help"){
				std::cout << "Commands:\n\n  run [what to run] [param1] [param2]..\n    Usage:\n" <<
							 "    run server [port]\n    run client [server ip] [server port]\n\n";
			}
		}
	}
	
}
