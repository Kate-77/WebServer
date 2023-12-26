#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include "HttpRequestParser.hpp"

int main()
{
    struct sockaddr_in a, client_addr;
	int server_socket, client_fd, bytes_received;
	socklen_t sin_size;
	char buf[1024];
	fd_set master;
    fd_set read_fds;
	int fdmax;
	int i;

	FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    memset(&a, 0, sizeof(a));

	//prepare sockaddr_in structure
    // a.sin_family = AF_INET;
    // a.sin_port = htons(5000);	//Change to port from config file
    // a.sin_addr.s_addr = htonl(INADDR_ANY); 
    // memset(a.sin_zero, '\0', sizeof(a.sin_zero));

	// //create socket
	// server_socket = socket(AF_INET, SOCK_STREAM, 0);
	// if (server_socket == -1)
	// {
	// 	perror("socket");
	// 	exit(1);
	// }

	// //bind socket
	// int yes = 1;
	// 	// reuse of the same port even if it is in the TIME_WAIT state from a previous connection
	// if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	// {
	// 	perror("setsockopt");
	// 	exit(1);
	// }
	// if (bind(server_socket, (struct sockaddr *)&a, sizeof(a)) == -1)
	// {
	// 	perror("bind");
	// 	exit(1);
	// }

	// //listen socket
	// if (listen(server_socket, SOMAXCONN) == -1)
	// {
	// 	perror("listen");
	// 	exit(1);
	// }
	// std::cout << "Server is listening on port 5000" << std::endl;

	// FD_SET(server_socket, &master);
	// fdmax = server_socket;

	while (42)
	{
		// read_fds = master;
		// if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		// {
		// 	perror("select");
		// 	exit(1);
		// }
		// run through the existing connections looking for data to read
		for (i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &read_fds)) // we got one
			{
				if (i == server_socket)	// handle new connections
				{
					sin_size = sizeof(client_addr);
					client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
					if (client_fd == -1)
						perror("accept");
					else
					{
						FD_SET(client_fd, &master); // add to master set
						if (client_fd > fdmax)	// keep track of the max
							fdmax = client_fd;
						std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << " on socket " << client_fd << std::endl;
					}
				}
				else
				{
						// handle data from a client
					// read request
					bytes_received = recv(i, buf, sizeof(buf), 0);
					if (bytes_received <= 0)
					{
						if (bytes_received == 0)
						{
							std::cout << "Connection closed by client on socket " << i << std::endl;
						}
						else
						{
							perror("recv");
						}
						close(i);
						FD_CLR(i, &master); // remove from master set
					}
					else
					{
							// printf("bytes received: %d\n", bytes_received);
						// printf("buf: %s\n", buf);

						// parse request
						// HttpRequestParser parser;
						// parser.parseRequest(buf);
						// parser.printParsedInfo();

						char clientIP[INET_ADDRSTRLEN];
						inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, sizeof(clientIP));
						std::cout << "server: got connection from " << clientIP << std::endl;
						
						// send response

						std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, world!";
						if (send(i, httpResponse.c_str(), httpResponse.length(), 0) == -1)
							perror("send");
						close(i);
						FD_CLR(i, &master);
						// continue;f
					}
				}
			}
		}
	}
	close(server_socket);
	
    return 0;
}
