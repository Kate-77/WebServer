// #include "WebServ.hpp"
// #include "Request.hpp"
// #include "Response.hpp"

// int main(int argc, char **argv)
// {
//     WebServ     server;
//     Request     request;
//     Response    response;

//     server.setConfig(argv[1]);
//     request.setRequest(argv[2]);    
//     response.setResponse(request, server);
//     std::cout << response.getResponse() << std::endl;
//     return (0);
// }

#include <iostream>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

void sendHttpResponse(int clientSocket, const std::string& content) {
    // HTTP response headers
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n";
    responseStream << "Content-Type: text/html\r\n";
    responseStream << "Content-Length: " << content.length() << "\r\n";
    responseStream << "\r\n"; // Blank line indicating the end of headers
    responseStream << content;

    // Send the response to the client
    std::string response = responseStream.str();
    send(clientSocket, response.c_str(), response.length(), 0);
}

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080); // Use any available port

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        // Accept a client connection
        sockaddr_in clientAddress{};
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);

        if (clientSocket == -1) {
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // Send a simple HTTP response to the client
        std::string responseContent = "<html><body>Hello, World!</body></html>";
        sendHttpResponse(clientSocket, responseContent);

        // Close the client socket
        close(clientSocket);
        std::cout << "Client disconnected" << std::endl;
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
