#ifndef SOCKET_CUSTOM_H
#define SOCKET_CUSTOM_H

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

struct custom_socket{
    SOCKET ClientSocket;
    SOCKET ListenSocket;
    uint16_t Port;
    uint16_t BufferSize;
    bool Connected;
    bool Stopped;
    char *Packet;
};

int init_custom_socket(struct custom_socket *CustomSocket, uint16_t Port);
int accept_and_stream_async_custom_socket(struct custom_socket *CustomSocket);
int clean_custom_socket(struct custom_socket *CustomSocket);

static int run_accept_and_stream_custom_socket(void *data);

void socket04();

#endif