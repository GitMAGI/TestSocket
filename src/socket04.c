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

#include <SDL2/SDL_thread.h>

int init_custom_socket(struct custom_socket *CustomSocket, uint16_t Port){
    int iResult;
    struct sockaddr_in serv_addr;

    CustomSocket = (struct custom_socket *)malloc(sizeof(struct custom_socket));
    if(!CustomSocket){
        return -1;
    }

    CustomSocket->Connected = false;
    CustomSocket->Stopped = true;

    #if defined _WIN64 || defined _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2,2), &wsaData);
        if(iResult != 0){
            CustomSocket->Connected = false;
            CustomSocket->Stopped = true;
            criticalLog("WSAStartup failed with error: %d", iResult);
            return -1;
        }
    #endif

    // Create a SOCKET for connecting to server
    CustomSocket->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (CustomSocket->ListenSocket == INVALID_SOCKET) {
        criticalLog("Listening Socket Creation failed");
        CustomSocket->Connected = false;
        CustomSocket->Stopped = true;
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
        CustomSocket->Connected = false;
        CustomSocket->Stopped = true;
        closesocket(CustomSocket->ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return -1;
    }
    
    iResult = listen(CustomSocket->ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        criticalLog("Listening Socket listen failed");
        CustomSocket->Connected = false;
        CustomSocket->Stopped = true;
        closesocket(CustomSocket->ListenSocket);
        #if defined _WIN64 || defined _WIN32            
            WSACleanup();
        #endif
        return -1;
    }

    CustomSocket->Connected = false;
    CustomSocket->Stopped = false;

    debugLog("Connected %d", CustomSocket->Connected);
    debugLog("Stopped %d", CustomSocket->Stopped);
    debugLog("Port %d", CustomSocket->Port);

    return 0;
}

int accept_and_stream_custom_socket(struct custom_socket *CustomSocket){
    if(!CustomSocket){
        errorLog("CustomSocket must be initialized.");
        return -1;
    }

    debugLog("Pre looping");

    // Loop listening and pushing data
    do{
        struct timeval st_call, et_call;
        getTick(&st_call);

        //Log listening on port and ip
        struct sockaddr_in serv_addr_;
        socklen_t serv_addr_len_ = sizeof(struct sockaddr_in);
        getsockname(CustomSocket->ListenSocket, (struct sockaddr *)&serv_addr_, &serv_addr_len_);
        char *serv_ip = inet_ntoa(serv_addr_.sin_addr);
        uint16_t serv_port = htons (serv_addr_.sin_port);
        infoLog("Host listening on %s:%d", serv_ip, serv_port);

        // Accept a client socket
        CustomSocket->ClientSocket = accept(CustomSocket->ListenSocket, NULL, NULL);
        if (CustomSocket->ClientSocket == INVALID_SOCKET) {
            criticalLog("Accepting Socket Accept failed");
            CustomSocket->Connected = false;
            closesocket(CustomSocket->ClientSocket);            
            break;
        }

        //Log Accepted remote ip and port
        struct sockaddr_in clnt_addr_;
        socklen_t clnt_addr_len_ = sizeof(struct sockaddr_in);
        getsockname(CustomSocket->ClientSocket, (struct sockaddr *)&clnt_addr_, &clnt_addr_len_);
        char *clnt_ip = inet_ntoa(clnt_addr_.sin_addr);
        uint16_t clnt_port = htons (clnt_addr_.sin_port);
        infoLog("Accepted %s remote host on port %d", clnt_ip, clnt_port);

        int iSendResult = 0;
        do{
            iSendResult = send( CustomSocket->ClientSocket, CustomSocket->Packect, CustomSocket->BufferSize, 0 );
            if (iSendResult == SOCKET_ERROR) {
                errorLog("Client Socket Sending failed");
                CustomSocket->Connected = false;
                break;
            }
            else
                debugLog("Client Socket %d Bytes sent", iSendResult);

            uint16_t deelay_ = 500;
            #if defined _WIN64 || defined _WIN32
                Sleep(deelay_);
            #else
                sleep(deelay_);
            #endif
        } while(!CustomSocket->Stopped);

        // shutdown the connection since we're done
        int iResult = shutdown(CustomSocket->ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            criticalLog("Client Socket Shutdown failed");
            CustomSocket->Connected = false;
            closesocket(CustomSocket->ClientSocket);            
            break;
        }

        // cleanup
        closesocket(CustomSocket->ClientSocket);
        getTick(&et_call);
        infoLog("Disconnected remote host %s on port %d. ETA %s", clnt_ip, clnt_port, getETA(st_call, et_call));

    } while(CustomSocket && CustomSocket->Connected);
}

int clean_custom_socket(struct custom_socket *CustomSocket){
    if(CustomSocket){
        CustomSocket->Stopped = true;
        CustomSocket->Connected = false;
        // cleanup
        closesocket(CustomSocket->ListenSocket);
    }
    #if defined _WIN64 || defined _WIN32
        WSACleanup();
    #endif
}

static int run_accept_and_stream_custom_socket(void *data) {
    struct custom_socket *CustomSocket = data;
    return accept_and_stream_custom_socket(CustomSocket);
}

void socket04(){
    struct timeval st, et;
    getTick(&st);
    infoLog("Socekt TEST 04 - Starting ...");

    struct custom_socket CustomSocket;
    uint16_t Port = DEFAULT_PORT;

    if(init_custom_socket(&CustomSocket, Port) < 0){        
        errorLog("Custom socket initialization failed");
        goto end;
    }

    debugLog("After Initialization");
    debugLog("After Initialization. Connected %d", CustomSocket.Connected);
    debugLog("After Initialization. Stopped %d", CustomSocket.Stopped);
    debugLog("After Initialization. Port %d", CustomSocket.Port);
    if(CustomSocket.ListenSocket)
        debugLog("After Initialization. Listen Socket Exists");

    goto end;

    debugLog("Pre Main Execution");
    //SDL_Thread *thread = SDL_CreateThread(run_accept_and_stream_custom_socket, "customsocket", CustomSocket);
    accept_and_stream_custom_socket(&CustomSocket);
    debugLog("Main Execution Completed!");

    if(clean_custom_socket(&CustomSocket) < 0){        
        errorLog("Custom socket cleaning failed");
        goto end;
    }        

    char *ch;
    scanf("Press a key to Exit %c", ch);
    
    end:
        getTick(&et);
        infoLog("Socekt TEST 04 - Completed. ETA %s", getETA(st, et));
}