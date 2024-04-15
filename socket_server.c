#include <stdio.h>          // Standard input/output functions
#include <sys/socket.h>     // Socket functions and structures
#include <sys/types.h>      // Data types used in system calls
#include <netinet/in.h>     // Internet address family structures
#include <arpa/inet.h>      // Functions for manipulating IP addresses
#include <unistd.h>         // POSIX operating system API
#include <stdlib.h>         // Standard library functions

// Define constants for port number and buffer size
#define PORT 9001
#define BUFFER_SIZE 500

int main() {
    // Declare variables for server and client sockets
    int server_socket, client_socket;

    // Declare structures for server address and data buffers
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = "hello from the server!";
    char recv_buffer[BUFFER_SIZE];

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket"); // Print error message if socket creation fails
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;            // Set address family to IPv4
    server_addr.sin_port = htons(PORT);          // Convert port number to network byte order
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.2"); // Set IP address

    // Bind the socket to a specific IP and port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket"); // Print error message if binding fails
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Error listening"); // Print error message if listening fails
        exit(EXIT_FAILURE);
    }

    // Accept a client connection
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
        perror("Error accepting connection"); // Print error message if accepting connection fails
        exit(EXIT_FAILURE);
    }
    printf("Client connected!\n");

    // Send data to the client
    if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
        perror("Error sending data"); // Print error message if sending data fails
        exit(EXIT_FAILURE);
    }

    // Receive data from the client
    if (recv(client_socket, recv_buffer, sizeof(recv_buffer), 0) < 0) {
        perror("Error receiving data"); // Print error message if receiving data fails
        exit(EXIT_FAILURE);
    }
    printf("Client has sent: %s\n", recv_buffer);

    // Close client and server sockets
    close(client_socket);
    close(server_socket);
    return 0;
}
