#include "./server.h"

Server::Server()
{
	init();
}

Server::~Server()
{
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

void Server::init()
{

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        exit(-1);
    }

    createServerSocket();
    bindToServerSocket();
    startListening();

    acceptConnections();
    

    
}

void Server::createServerSocket()
{
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        exit(-1);
    }
}

void Server::bindToServerSocket() {
    // Bind socket
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(-1);
    }
}

void Server::startListening()
{
    // Listen for connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(-1);
    }

    std::cout << "Waiting for connections on port 8080..." << std::endl;
}

void Server::acceptConnections() {
    std::cout << "Server Ready and RUNNING" << std::endl;

    while (true) {

        // Accept client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed!" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            exit(-1);
        }


        std::thread(handleClient, clientSocket).detach();
    }
}

std::string Server::readFile(std::string f)
{
    std::ifstream file(f);
    if (!file) return "<h1>404 Not Found</h1>";

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void Server::handleClient(SOCKET client)
{
    std::cout << "Handling Client using new Thread!" << std::endl;
    char buffer[1024] = { 0 };

    // Receive data from client
    int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received:\n\n " << buffer << std::endl;

        // Send response
        std::string htmlContent = readFile("index.html");
        std::string httpResponse =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(htmlContent.length()) + "\r\n"
            "\r\n" + htmlContent;
        send(client, httpResponse.c_str(), httpResponse.length(), 0);
    }

    closesocket(client);
}
