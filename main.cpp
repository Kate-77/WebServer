#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>

void sendResponse(int new_fd)
{
	std::string _responseBuffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world!</h1></body></html>";
	send(new_fd, _responseBuffer.c_str(), _responseBuffer.length(), 0);
	close(new_fd);
}


int main()
{
	// create socket
	// socketaa
	// create 
    struct sockaddr_in a;
	int sockfd;

    memset(&a, 0, sizeof(a));

	//prepare sockaddr_in structure
    a.sin_family = AF_INET;
    a.sin_port = htons(1234);	//Change to port from config file
    a.sin_addr.s_addr = htonl(INADDR_ANY); 
    memset(a.sin_zero, '\0', sizeof(a.sin_zero));

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket");
		exit(1);
	}

	//bind socket
	int yes = 1;
		// reuse of the same port even if it is in the TIME_WAIT state from a previous connection
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
	if (bind(sockfd, (struct sockaddr *)&a, sizeof(a)) == -1)
	{
		perror("bind");
		exit(1);
	}

	//listen socket
	if (listen(sockfd, SOMAXCONN) == -1)
	{
		perror("listen");
		exit(1);
	}


	//accept client
	int new_fd;
	socklen_t sin_size;
	struct sockaddr_in client_addr;
	sin_size = sizeof(client_addr);
	new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
	if (new_fd == -1)
	{
		perror("accept");
		exit(1);
	}

	// read request
	char buf[1024];

	int bytes_received = recv(new_fd, buf, sizeof(buf), 0);
	// init parse request
	// parseLine(buf, bytes_received)

	printf("bytes received: %d\n", bytes_received);
	printf("buf: %s\n", buf);

	sendResponse(new_fd);
// // std::cout << "Server: Connection accepted from " << std::endl;

// 	char clientIP[INET_ADDRSTRLEN];
//     inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, INET_ADDRSTRLEN);
//     std::cout << "Server: Connection accepted from " << clientIP << ":" << ntohs(client_addr.sin_port) << std::endl;


// 	const char* message = "Hello, world!";
//     if (send(new_fd, message, strlen(message), 0) == -1) {
//         perror("send");
//         close(new_fd);
//         close(sockfd);
//         exit(EXIT_FAILURE);
//     }

// 	char buffer[1024];
//     ssize_t bytes_received = recv(new_fd, buffer, sizeof(buffer), 0);
//     if (bytes_received == -1) {
//         perror("recv");
//         close(new_fd);
//         close(sockfd);
//         exit(EXIT_FAILURE);
//     } else if (bytes_received == 0) {
//         std::cout << "Client disconnected" << std::endl;
//     } else {
//         buffer[bytes_received] = '\0';
//         std::cout << "Received from client: " << buffer << std::endl;
//     }

//     // Close sockets when done
//     close(new_fd);
//     close(sockfd);

    return 0;
}


// pppppppp

// 6\r\n
// Hello,
// world!\r\n
// \r\n