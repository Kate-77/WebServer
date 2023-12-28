#include "Server.hpp"




void Server::handleServers(std::vector<std::pair<Socket, Parser *> > & servers)
{
	//add socket_fd to master read set
	add_servers(servers, master_read_fds);

	while(42)
	{
		tmp_read_fds = master_read_fds;
		tmp_write_fds = master_write_fds;
		if (select(fdmax+1, &tmp_read_fds, &tmp_write_fds, NULL, NULL) == -1)
		{
			perror("select");
			exit(1);
		}
		
		// handle new connections
		handle_new_connections(servers, tmp_read_fds);



		// run through the existing connections looking for data to read
		
				// else
				// {
				// 	// handle data from a client
				// 	int nbytes;
				// 	char buf[1024];
				// 	if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
				// 	{
				// 		// got error or connection closed by client
				// 		if (nbytes == 0)	// connection closed
				// 			std::cout << "Connection closed by client on socket" << i << " !!hung up!!" << std::endl;
				// 		else
				// 			perror("recv");
				// 		close(i);
				// 		FD_CLR(i, &master_read_fds); // remove from master set
				// 	}
				// 	else
				// 	{
				// 		// we got some data from a client
				// 		std::cout << "Received data from client on socket " << i << std::endl;
				// 		std::cout << buf << std::endl;
				// 		// send(i, buf, nbytes, 0);
				// 		std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, world!";
				// 		if (send(i, httpResponse.c_str(), httpResponse.length(), 0) == -1)
				// 			perror("send");
				// 		close(i);
				// 		FD_CLR(i, &master_read_fds);
				// 	}
				// }
			
	}

}

	


void Server::add_servers(std::vector<std::pair<Socket, Parser *>> &servers, fd_set &master_read_fds)
{
	for(std::vector<std::pair<Socket, Parser *> >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		FD_SET((*it).first.getServerSocket(), &master_read_fds);
		if ((*it).first.getServerSocket() > fdmax)
			fdmax = (*it).first.getServerSocket();
	}
}

void Server::handle_new_connections(std::vector<std::pair<Socket, Parser *>> &servers, fd_set &tmp_read_fds)
{
	for(std::vector<std::pair<Socket, Parser *> >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		if (FD_ISSET((*it).first.getServerSocket(), &tmp_read_fds))
		{
			Client client;
			socklen_t sin_size;
			struct sockaddr_in client_addr;
			sin_size = sizeof(client_addr);
			int client_socket = accept((*it).first.getServerSocket(), (struct sockaddr *)&client_addr, &sin_size);
			client.setClientSocket(client_socket);
			if (client_socket == -1)
				perror("accept");
			else
			{
				FD_SET(client_socket, &master_read_fds); // add to master set
				if (client_socket > fdmax)	// keep track of the max
					fdmax = client_socket;
				this->clients.push_back(std::make_pair(client, (*it).second));
			}
		}
	}
}

Server::Server()
{
	FD_ZERO(&master_read_fds);
	FD_ZERO(&master_write_fds);
	fdmax = 0;
}

Server::~Server() { }