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

#include "log.h"
#include "util.h"
#include "config.h"
#include "socket03.h"

void socket03(){
    struct timeval st, et;
    getTick(&st);
    infoLog("Socekt TEST 03 - Starting ...");

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

    // Loop listening and pushing data
    do{
        struct timeval st_call, et_call;
        getTick(&st_call);

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
            closesocket(ClientSocket);            
            break;
        }

        //Log Accepted remote ip and port
        struct sockaddr_in clnt_addr_;
        socklen_t clnt_addr_len_ = sizeof(struct sockaddr_in);
        getsockname(ClientSocket, (struct sockaddr *)&clnt_addr_, &clnt_addr_len_);
        char *clnt_ip = inet_ntoa(clnt_addr_.sin_addr);
        uint16_t clnt_port = htons (clnt_addr_.sin_port);
        infoLog(ssprintf("Accepted %s remote host on port %d", clnt_ip, clnt_port));

        // Send until the peer shuts down the connection
        int iSendResult = 0;
        do{
            char *fakeData = randomString(16);
            iSendResult = send( ClientSocket, fakeData, strlen(fakeData), 0 );
            if (iSendResult == SOCKET_ERROR) {
                errorLog("Client Socket Sending failed");
                break;
            }
            else
                debugLog(ssprintf("Client Socket %d Bytes sent", iSendResult));

            uint16_t deelay_ = 500;
            #if defined _WIN64 || defined _WIN32
                Sleep(deelay_);
            #else
                sleep(deelay_);
            #endif
        } while(true);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            criticalLog("Client Socket Shutdown failed");
            closesocket(ClientSocket);            
            break;
        }

        // cleanup
        closesocket(ClientSocket);
        getTick(&et_call);
        infoLog(ssprintf("Disconnected remote host %s on port %d. ETA %s", clnt_ip, clnt_port, getETA(st_call, et_call)));

    } while(true);

    // cleanup
    closesocket(ListenSocket);
    #if defined _WIN64 || defined _WIN32
        WSACleanup();
    #endif

    getTick(&et);
    infoLog(ssprintf("Socekt TEST 03 - Completed. ETA %s", getETA(st, et)));
}