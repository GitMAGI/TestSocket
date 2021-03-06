#if defined _WIN64 || defined _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
#else

#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "log.h"
#include "util.h"
#include "config.h"
#include "socket01.h"

void socket01(){
    struct timeval st, et;
    getTick(&st);
    infoLog("Socekt TEST 01 - Starting ...");

    int iResult;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char recvbuf[DEFAULT_BUFLEN];

    #if defined _WIN64 || defined _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2,2), &wsaData);
        if(iResult != 0){
            criticalLog(ssprintf("WSAStartup failed with error: %d", iResult));
            return;
        }        
    #endif

    // Create a SOCKET for connecting to server
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        criticalLog("Listening Socket Creation failed");
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return;
    }

    // Setup the TCP listening socket
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(DEFAULT_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    iResult = bind(ListenSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (iResult < 0){
        criticalLog(ssprintf("Listening Socket bind failed with error: %d", iResult));
        closesocket(ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return;
    }

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        criticalLog("Listening Socket listen failed");
        closesocket(ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return;
    }

    //Log listening on port and ip
    struct sockaddr_in serv_addr_;
    socklen_t serv_addr_len_ = sizeof(struct sockaddr_in);
    getsockname(ListenSocket, (struct sockaddr *)&serv_addr_, &serv_addr_len_);
    char *serv_ip = inet_ntoa(serv_addr_.sin_addr);
    uint16_t serv_port = htons (serv_addr_.sin_port);
    infoLog(ssprintf("Host listening on %s:%d", serv_ip, serv_port));

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        criticalLog("Accepting Socket Accept failed");
        closesocket(ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return;
    }

    //Log Accepted remote ip and port
    struct sockaddr_in clnt_addr_;
    socklen_t clnt_addr_len_ = sizeof(struct sockaddr_in);
    getsockname(ClientSocket, (struct sockaddr *)&clnt_addr_, &clnt_addr_len_);
    char *clnt_ip = inet_ntoa(clnt_addr_.sin_addr);
    uint16_t clnt_port = htons (clnt_addr_.sin_port);
    infoLog(ssprintf("Accepted %s remote host on port %d", clnt_ip, clnt_port));

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {
        iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            infoLog(ssprintf("Client Socket Bytes received: %d", iResult));
        }
        else if (iResult == 0)
            infoLog("Client Socket Connection closing ...");
        else  {
            criticalLog("Client Socket Receiving failed");
            closesocket(ClientSocket);
            #if defined _WIN64 || defined _WIN32            
                WSACleanup();
            #endif
            break;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        criticalLog("Client Socket Shutdown failed");
        closesocket(ClientSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return;
    }

    // cleanup
    closesocket(ClientSocket);
    #if defined _WIN64 || defined _WIN32
        WSACleanup();
    #endif

    getTick(&et);
    infoLog(ssprintf("Socekt TEST 01 - Completed. ETA %s", getETA(st, et)));
}