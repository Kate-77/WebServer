#include "Server.hpp"


void Server::handleServers(Socket socket)
{
	FD_SET(socket.getServerSocket(), &master_read_fds);
	fdmax = socket.getServerSocket();

	while(42)
	{
		tmp_read_fds = master_read_fds;
		tmp_write_fds = master_write_fds;
		if (select(fdmax+1, &tmp_read_fds, &tmp_write_fds, NULL, NULL) == -1)
		{
			perror("select");
			exit(1);
		}

		// run through the existing connections looking for data to read
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &tmp_read_fds)) // we got one
			{
				if (i == socket.getServerSocket())	// handle new connections
				{
					Client client;
					socklen_t sin_size;
					struct sockaddr_in client_addr = client.getClientAddress();
					sin_size = sizeof(client_addr);
					int client_socket = accept(socket.getServerSocket(), (struct sockaddr *)&client_addr, &sin_size);
					client.setClientSocket(client_socket);
					if (client_socket == -1)
						perror("accept");
					else
					{
						FD_SET(client_socket, &master_read_fds); // add to master set
						if (client_socket > fdmax)	// keep track of the max
							fdmax = client_socket;
					}
				}
				else
				{
					// handle data from a client
					int nbytes;
					char buf[1024];
					if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
					{
						// got error or connection closed by client
						if (nbytes == 0)	// connection closed
							std::cout << "Connection closed by client on socket" << i << " !!hung up!!" << std::endl;
						else
							perror("recv");
						close(i);
						FD_CLR(i, &master_read_fds); // remove from master set
					}
					else
					{
						// we got some data from a client
						std::cout << "Received data from client on socket " << i << std::endl;
						std::cout << buf << std::endl;
						// send(i, buf, nbytes, 0);
						std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, world!";
						if (send(i, httpResponse.c_str(), httpResponse.length(), 0) == -1)
							perror("send");
						close(i);
						FD_CLR(i, &master_read_fds);
					}
				}
			}
		}

	}

	

}
Server::Server()
{
}

Server::~Server() { }