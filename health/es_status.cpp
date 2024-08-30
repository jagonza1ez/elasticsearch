/*
* This program uses a socket to connect, get and receive the health status
* of the elasticsearch container. It parses and extracts that health status
* and prints the result. Closes the socket to free up resources.
*/

#include "socket_utils.h"
#include <iostream>
#include <unistd.h>

// Function to extract the health status of the es container
std::string extract_status(const std::string& response) {
    
    // FInds the status in response
    std::size_t pos = response.find("\"status\":\"");
    
    // Conditional statement if the find function cannot the string status.
    if (pos != std::string::npos) {
        
        // Calc the starting position of status, 10 indices long "status":"
        std::size_t start = pos + 10;
        
        // Finds the end of the status string, double quote
        std::size_t end = response.find("\"", start);
        
        // extracts the status, "green", "yellow", "red" from the response string
        return response.substr(start, end - start);
    }
    return "Unknown";
}

// main functions - returns integer if the program runs succesfully
int main() {
    
    // Declares a constant pointer that points to the ip for elasticsearch
    // you can verify using ip a
    const char* elasticsearch_host = "127.0.0.1";
    int elasticsearch_port = 9200;
    
    // calls the create_socket from the socket_utils.cpp
    // connects to the elasticsearch container.
    int sock = create_socket(elasticsearch_host, elasticsearch_port);
    
    // conditional statement to check if the socket was created successfully
    // if it is less than 0 then returns 1 and the program exits.
    if (sock < 0) {
        return 1;
    }

    // Creates the variable request, contains the HTTP GET request.
    // The request pertains to the elasticsearch health. 
    std::string request = "GET /_cluster/health HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
    
    // Creates the variable that calls the send_http_request function also
    // defined in the socket_utils.cpp, uses the socket to send and receive the
    // HTTP request and stores it in the response variable.
    std::string response = send_http_request(sock, request);

    // closes the socket.
    close(sock);

    // Creates the status variable, call the extract_status and parses and extracts the 
    // reponse variable to store it inside status.
    std::string status = extract_status(response);

    // Prints the result.
    std::cout << "Elasticsearch Health Status: " << status << std::endl;

    return 0;
}
