#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#include <thread>

#include "client.h"
#include "shared.h"

#include "ui\chat_ui.h"

#define DEFAULT_BUFLEN 4096

void runClient(const char* ip, const char* port){
	std::cout << "Starting client:\n";
	system("title Client");
	//
	
	std::cout << "Connecting to " << ip << "'s " << port << "...\n";
	
	WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
    	std::cout << "WSAStartup failed with error:" << iResult << " [Fatal]\n";
        return;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ip, port, &hints, &result);
    if ( iResult != 0 ) {
    	std::cout << "getaddrinfo failed with error:" << iResult << " [Fatal]\n";
        WSACleanup();
        return;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            std::cout << "Socket failed with error:" << WSAGetLastError() << " [Fatal]\n";
            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Couldn't connect to the server! [Fatal]\n";
        WSACleanup();
        return;
    }
    
    
    //NEW SYSTEM
    ClientChat chat;
    
    chat.setConnectSocket(ConnectSocket);
    state end_state = chat.BringChatUI();
    
    //End state.
    switch(end_state){
    	default:{
    		
			break;
		}
	}
    
    
    //Create and run thread for incoming messages
    
    //std::thread recive(readMessages, ConnectSocket);
    
    
    /*
    //Send loop
    std::string input;
    unsigned int current_length;
    for(;;){
    	std::getline(std::cin, input);
    	
    	current_length = input.length();
    	if(current_length>0&&current_length<DEFAULT_BUFLEN){
    		if(input=="/disconnect"){
    			//Means user requested disconnect.
    			//So we cleanup.
    			closesocket(ConnectSocket);
    			WSACleanup();
    			recive.join();
    			
    			std::cout << "User disconnect.\n";
    			return;
			}
    		char current_buff[DEFAULT_BUFLEN];
    		
    		for(unsigned int i = 0; i < current_length; i++){
    			current_buff[i] = input[i];
			}
			iResult = send( ConnectSocket, current_buff, current_length, 0 );
    	
   			if (iResult == SOCKET_ERROR) {
        		printf("send failed with error: %d\n", WSAGetLastError());
        		closesocket(ConnectSocket);
        		WSACleanup();
        		recive.join();
        		return;
    		}
		}
    	
    	
	}
	*/
    
    //We never get down there at the moment.

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    //recive.join();
}
