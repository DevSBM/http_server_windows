#include "./client.h"

Client::Client()
{
	init();
}

Client::~Client()
{
    closesocket(clientSocket);
    WSACleanup();
    exit(0);
}

void Client::init()
{
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        exit(-1);
    }

    createSocket();
    connectToServer();
    sendMessageToServer();
    waitForResponse();
}

void Client::createSocket()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        exit(-1);
    }
}

void Client::connectToServer()
{
    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(-1);
    }

    std::cout << "Connected to server!" << std::endl;
}

void Client::sendMessageToServer()
{
    // Send message to server
    const char* message = "GET / HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        "Connection: close\r\n"
        "\r\n";
    send(clientSocket, message, strlen(message), 0);
}

void Client::waitForResponse()
{
    // Receive server response
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }
}
