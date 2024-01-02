#include "Server.hpp"

Server::Server() {
	FD_ZERO(&master_read_fds);
	FD_ZERO(&master_write_fds);
	FD_ZERO(&tmp_read_fds);
	FD_ZERO(&tmp_write_fds);
	fdmax = 0;
}

Server::~Server() {}

Server::Server(const Server &server) {
	*this = server;
}

Server &Server::operator=(const Server &server) {
	this->master_read_fds = server.master_read_fds;
	this->master_write_fds = server.master_write_fds;
	this->tmp_read_fds = server.tmp_read_fds;
	this->tmp_write_fds = server.tmp_write_fds;
	this->fdmax = server.fdmax;
	this->clients = server.clients;
	return *this;
}

fd_set Server::getMasterReadFds() {
	return this->master_read_fds;
}

fd_set Server::getMasterWriteFds() {
	return this->master_write_fds;
}

fd_set Server::getTmpReadFds() {
	return this->tmp_read_fds;
}

fd_set Server::getTmpWriteFds() {
	return this->tmp_write_fds;
}

int Server::getFdmax() {
	return this->fdmax;
}

std::vector<std::pair<Client, Parser *> > Server::getClients() {
	return this->clients;
}

void Server::setMasterReadFds(fd_set master_read_fds) {
	this->master_read_fds = master_read_fds;
}

void Server::setMasterWriteFds(fd_set master_write_fds) {
	this->master_write_fds = master_write_fds;
}

void Server::setTmpReadFds(fd_set tmp_read_fds) {
	this->tmp_read_fds = tmp_read_fds;
}

void Server::setTmpWriteFds(fd_set tmp_write_fds) {
	this->tmp_write_fds = tmp_write_fds;
}

void Server::setFdmax(int fdmax) {
	this->fdmax = fdmax;
}

void Server::setClients(std::vector<std::pair<Client, Parser *> > clients) {
	this->clients = clients;
}



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
		
		// run through the existing client connections looking for data to read or write
		for(size_t i = 0; i < clients.size(); i++)
		{
			//check for read
			if (FD_ISSET(clients[i].first.getClientSocket(), &tmp_read_fds))
			{
				printf("read\n");
				unsigned char buf[1024];
				ssize_t nbytes;
				
				if ((nbytes = recv(clients[i].first.getClientSocket(), buf, sizeof(buf), 0)) <= 0)
				{
					// handle recv error
					handle_recv_err(clients[i].first.getClientSocket(), nbytes, i, master_read_fds, master_write_fds);
					break;
				}
				else
					//parse request
					parse_req(clients[i].first, buf, nbytes, master_read_fds, master_write_fds);
			}
			//check for write
			if (FD_ISSET(clients[i].first.getClientSocket(), &tmp_write_fds))
			{
				clients[i].first.response.client_fd = clients[i].first.getClientSocket();

				if(!clients[i].first.response.res_initialized)
				{
					// clients[i].first.response.handleResponse(clients[i].first.request , *clients[i].second);
					clients[i].first.response.res_initialized = true;
				}

				if (clients[i].first.response.client_done)
				{
					FD_CLR(clients[i].first.getClientSocket(), &master_write_fds);
					close(clients[i].first.getClientSocket());
					clients[i].first.response._file_fd.close();
					if (access(clients[i].first.request.bodyFileName.c_str(), F_OK) != -1)
						remove(clients[i].first.request.bodyFileName.c_str());
					else
						std::cout << "Error deleting bodyfile file" << std::endl;
					clients.erase(clients.begin() + i);
					i--;
				}
				else
				{
					//send response
					resp_send(clients[i].first.response);
				}

			} 
			
		}		
	}

}

void Server::add_servers(std::vector<std::pair<Socket, Parser *> > &servers, fd_set &master_read_fds)
{
	for(std::vector<std::pair<Socket, Parser *> >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		FD_SET((*it).first.getServerSocket(), &master_read_fds);
		if ((*it).first.getServerSocket() > fdmax)
			fdmax = (*it).first.getServerSocket();
	}
}

void Server::handle_new_connections(std::vector<std::pair<Socket, Parser *> > &servers, fd_set &tmp_read_fds)
{
	for(std::vector<std::pair<Socket, Parser *> >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		if (FD_ISSET((*it).first.getServerSocket(), &tmp_read_fds))
		{
			Client client;
			socklen_t sin_size;
			client.response.client_done = false;
			client.response.res_initialized = false;
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

void Server::handle_recv_err(int socket, ssize_t nbytes, int i, fd_set &master_read_fds, fd_set &master_write_fds)
{
	// got error or connection closed by client
	if (nbytes == 0)	// connection closed
		{
			std::cout << "Connection closed by client "  << std::endl;
			FD_CLR(socket, &master_read_fds); // remove from master set
			FD_CLR(socket, &master_write_fds); // remove from master set
			close(socket);
			clients.erase(clients.begin() + i);
		}
	else
	{
		perror("recv");
	}
}

void Server::parse_req(Client &client, unsigned char *buf, ssize_t nbytes, fd_set &master_read_fds, fd_set &master_write_fds)
{
	int Done = 0;

	try {
		printf("parse\n");
		client.request.parseRequest(nbytes, buf, Done);
	}
	catch (int code) {
		client.request.setStatusCode(code);
		Done = 1;
	}
	  printf("code = %d\n", client.request.getStatusCode());
	if (Done == 1)
	{
		printf("Done\n");
		//move client to write set
		FD_CLR(client.getClientSocket(), &master_read_fds);
		FD_SET(client.getClientSocket(), &master_write_fds);
	}
}

void Server::resp_send(Response &response)
{
	const char* message = "Hello, world!";
    if (send(response.client_fd, message, strlen(message), 0) == -1) {
        perror("send***********");
        // close(new_fd);
        // close(sockfd);
        exit(EXIT_FAILURE);
    }
}
