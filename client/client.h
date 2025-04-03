#pragma once
#include <iostream>
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// 5 Steps
// Initialize WSA
// Create a socket
// Connect to the server
// Send and receive data
// Disconnect

// Only for testing purpose
// Primary purpose is to develop the web-server
// instead of client

class Client {
public:
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    Client();
    ~Client();
    void init();
    void createSocket();
    void connectToServer();
    void sendMessageToServer();
    void waitForResponse();
};