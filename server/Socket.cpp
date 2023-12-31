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

void Socket::createServers(const std::vector<Parser *>       *  servers, std::vector<std::pair<Socket, Parser *> > & final_servers) {
	for(std::vector<Parser *>::const_iterator it = servers->begin(); it != servers->end(); it++)
	{
		static int number = 0;
		std::cout << "server number n = " << number++ << std::endl;
		// std::cout << "    " << "address: " << (*it)->_address << " port: " << (*it)->_port << std::endl;
		std::cout << "address :" << (*it)->getHostnormal() << " Port: " << (*it)->getPortnormal() << std::endl;
		std::cout << "address casted :" << (*it)->getHost() << " Port casted: " << (*it)->getPort() << std::endl;
		server_address.sin_family = AF_INET;
		// server_address.sin_port = htons(5000);	//Change to (*it)->_port()
		// server_address.sin_port = htons((*it)->getPortnormal()); // you can use the casted one getPort()
		server_address.sin_port = (*it)->getPort(); 
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		// server_address.sin_addr.s_addr = inet_addr(((*it)->getHostnormal()).c_str()); // you can use the casted one gethost() //not working
		// server_address.sin_addr.s_addr = (*it)->getHost();  //not working
		// server_address.sin_addr.s_addr = inet_addr("10.32.116.45");
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
		std::cout << "Server created and listening" << std::endl;
		final_servers.push_back(std::make_pair(*this, *it));
	}
}

