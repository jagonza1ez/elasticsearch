/*
* Header file for socket_utils
*/

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

// Function declarations
int create_socket(const char* host, int port);
std::string send_http_request(int sock, const std::string& request);

#endif // SOCKET_UTILS_H
