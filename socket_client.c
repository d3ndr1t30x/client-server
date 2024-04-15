#include <stdio.h>
#include <stdbool.h>    // Include for bool, true, false
#include <WinSock2.h>   // Windows Sockets 2 API
#include <WS2tcpip.h>   // Definitions for TCP/IP protocols

#pragma comment(lib, "Ws2_32.lib") // Linker directive to include Ws2_32.lib

// Constants
const int BUFFER_SIZE = 500;        // Size of send and receive buffers
const int PORT_NUMBER = 9001;       // Port number to connect to
const char* SERVER_IP = "192.168.1.2";  // IP address of the server

// Function to initialize Winsock
bool InitializeWinsock(WSADATA* wsaData) {
    int result = WSAStartup(MAKEWORD(2, 2), wsaData); // Initialize Winsock with version 2.2
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return false;
    }
    return true;
}

// Function to create a socket
SOCKET CreateSocket() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create a TCP socket
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }
    return clientSocket;
}

// Function to connect to the server
bool ConnectToServer(SOCKET clientSocket, const struct sockaddr_in* serverAddr) {
    int result = connect(clientSocket, (const struct sockaddr*)serverAddr, sizeof(*serverAddr)); // Connect to the server
    if (result == SOCKET_ERROR) {
        printf("Failed to connect to server: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

int main() {
    WSADATA wsaData;            // Variable to hold Winsock data
    SOCKET clientSocket;        // Socket for communication with server
    struct sockaddr_in serverAddr; // Structure to hold server address information
    char sendData[BUFFER_SIZE] = "Hello from the client!"; // Buffer for sending data
    char recvData[BUFFER_SIZE];    // Buffer for receiving data

    // Initialize Winsock
    if (!InitializeWinsock(&wsaData)) // Pass the address of wsaData
        return 1;

    // Create a socket
    clientSocket = CreateSocket();
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // Fill in server address structure
    serverAddr.sin_family = AF_INET;            // Address family is IPv4
    serverAddr.sin_port = htons(PORT_NUMBER);   // Convert port number to network byte order
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // Convert IP address string to binary form

    // Connect to the server
    if (!ConnectToServer(clientSocket, &serverAddr)) {
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Now you can use the socket for sending and receiving data
    // ...

    // Cleanup
    closesocket(clientSocket);  // Close the socket
    WSACleanup();               // Cleanup Winsock resources

    return 0;
}
