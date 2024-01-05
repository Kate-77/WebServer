#include "Socket.hpp"


Socket::Socket() {
	this->server_socket = -1;
	memset(&this->server_address, 0, sizeof(this->server_address));
}

Socket::~Socket() {}

Socket::Socket(const Socket &socket) {
	*this = socket;
}

Socket &Socket::operator=(const Socket &socket) {
	this->server_socket = socket.server_socket;
	this->server_address = socket.server_address;
	return *this;
}

int Socket::getServerSocket() {
	return this->server_socket;
}

sockaddr_in Socket::getServerAddress() {
	return this->server_address;
}

void Socket::setServerSocket(int server_socket) {
	this->server_socket = server_socket;
}

void Socket::setServerAddress(sockaddr_in server_address) {
	this->server_address = server_address;
}

//Methods

void Socket::createServers(const std::vector<Parser *> *servers, std::vector<std::pair<Socket, Parser *> > &final_servers) {
	for(std::vector<Parser *>::const_iterator it = servers->begin(); it != servers->end(); it++)
	{
		server_address.sin_family = AF_INET;
		server_address.sin_port = (*it)->getPort(); 
		// server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_addr.s_addr = (*it)->getHost();
		memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

		//create socket
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == -1)
		{
			perror("socket");
			exit(1);
		}

		//bind socket
		int yes = 1;
			// reuse of the same port even if it is in the TIME_WAIT state from a previous connection
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}
		if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
		{
			perror("bind");
			exit(1);
		}

		//listen socket
		if (listen(server_socket, SOMAXCONN) == -1)
		{
			perror("listen");
			exit(1);
		}
		final_servers.push_back(std::make_pair(*this, *it));
	}
}
