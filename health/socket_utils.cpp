#include "socket_utils.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// defines the buffer size (memory allocation) and fixes it 4096, to receive data
// from the socket. 4096 is the standard for netowrks.
#define BUFFER_SIZE 4096

int create_socket(const char* host, int port) {
    int sock;
    struct sockaddr_in server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error!" << std::endl;
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &server.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported!" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        return -1;
    }

    return sock;
}

std::string send_http_request(int sock, const std::string& request) {
    char buffer[BUFFER_SIZE];
    std::string response;

    if (send(sock, request.c_str(), request.length(), 0) < 0) {
        std::cerr << "Failed to send request!" << std::endl;
        return "";
    }

    int bytes_received;
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        response += buffer;
    }

    if (bytes_received < 0) {
        std::cerr << "Failed to receive response!" << std::endl;
    }

    return response;
}
