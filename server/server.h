#pragma once
#include <iostream>
#include <thread>
#include <fstream> 
#include <string>
#include <sstream>
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// 7 Steps
// Initialize WSA
// Create a socket
// Bind the socket
// Listen on the socket
// Accept connections
// Send and Receive Data
// Disconnect

// Initialiase Winsocket DLL
// Winsocket Interface
// int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData);
// 
// Socket Interface
// SOCKET WSAAPI socket(int af,int type,int protocol);
// af: Address Family (AF_INET/AF_INET6)
// type: type specification --> TCP/UDP (SOCK_STREAM/SOCK_DGRAM/SOCK_RAW/SOCK_SEQPACKET)
// protocol: specifies the transport protocol (IPPROTO_TCP for TCP,IPPROTO_UDP for UDP)
// 
// WSACleanup must be called by application/DLL to deregister itshelf
// from Windows Sockets and to cleanup dll from shared memory

// Close Socket interface
// int closesocket(Socket s);

// Bind Interface -- Binding (IpAddr,PORT)
// int bind(SOCKET s,const struct sockaddr* name,int socklen);
// If binding is successful function returns 0 
// otherwise it return SOCKET_ERROR

// sockaddr_in: used to represent IPv4 address
//struct sockaddr_in {
//    sa_family_t sin_family;   // Address family (AF_INET for IPv4)
//    in_port_t sin_port;       // Port number (16-bit)
//    struct in_addr sin_addr;  // IPv4 address (32-bit)
//    char sin_zero[8];         // Padding (not used, should be zero)
//};
// Configure sockaddr_in
// serverAddr.sin_family = AF_INET;          // IPv4
// serverAddr.sin_port = htons(8080);        // Convert port to network byte order
// inet_pton(AF_INET, "192.168.1.100", &serverAddr.sin_addr);  // Convert IP
// While passing to a function -> (struct sockaddr*)&serverAddr

// listen interface
// int listen(SOCKET s,int backlog);
// backlog: maximum number of connections allowed
// If no error occurs listen returns zero otherwise
// a value of SOCKET_ERROR is returned

// WSAGetLastError(): Fetches the last error
// WSACleanup(): Frees the memory occupied by the memory

// accept interface
// SOCKET accept(SOCKET sockfd, struct sockaddr* addr, int* addrlen);
// sockfd: The listening socket descriptor (created using socket() and bound with bind()).
// addr: (optional argument can be NULL) Pointer to a sockaddr structure to store the client's address.
// addrlen:  (optional argument can be NULL) Pointer to an integer specifying the size of addr. It is updated with the actual size after accept()
// Its a blocking function
// This function returns another socket for client connection 
// Original socket remains intact for listening purposes

// connect interface (used by client)
// int connect(SOCKET sockfd,const struct sockaddr* addr,int addrlen);
// sockfd	The socket descriptor (created using socket())
// addr		Pointer to a sockaddr structure containing the server's IP and port.
// addrlen  Size in bytes of addr structure
// connect will wait at max 75s for server to respond

// Note:
// send() / recv() is used for TCP (after connection)
// sendto() / recvfrom() is used for UDP (specify address)


class Server {
public:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	int addrLen = sizeof(clientAddr);

	Server();
	~Server();
	void init();
	void createServerSocket();
	void bindToServerSocket();
	void startListening();
	void acceptConnections();
	
	static std::string readFile(std::string f);
	static void handleClient(SOCKET client);
};