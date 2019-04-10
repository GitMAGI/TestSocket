#include "socket04.h"
#include "log.h"
#include "util.h"

#if defined _WIN64 || defined _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
#else
    #include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int init_custom_socket(struct custom_socket *CustomSocket, uint16_t Port){
    int iResult;
    struct sockaddr_in serv_addr;

    CustomSocket = (struct custom_socket *)malloc(sizeof(struct custom_socket));
    if(!CustomSocket){
        return -1;
    }

    #if defined _WIN64 || defined _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2,2), &wsaData);
        if(iResult != 0){
            criticalLog("WSAStartup failed with error: %d", iResult);
            return -1;
        }        
    #endif

    // Create a SOCKET for connecting to server
    CustomSocket->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (CustomSocket->ListenSocket == INVALID_SOCKET) {
        criticalLog("Listening Socket Creation failed");
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return -1;
    }
    
    // Setup the TCP listening socket
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(Port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    iResult = bind(CustomSocket->ListenSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (iResult < 0){
        criticalLog("Listening Socket bind failed with error: %d", iResult);
        closesocket(CustomSocket->ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return -1;
    }
    
    iResult = listen(CustomSocket->ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        criticalLog("Listening Socket listen failed");
        closesocket(CustomSocket->ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return -1;
    }

    return 0;
}