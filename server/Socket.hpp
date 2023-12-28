#pragma once
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "../conf/Parser.hpp"
#include <arpa/inet.h>

class Socket
{
	private:
		int server_socket;
		struct sockaddr_in server_address;

	public:
		//canonical form
		Socket();
		~Socket();
		Socket(const Socket &socket);
		Socket& operator=(const Socket &socket);

		//getters
		int getServerSocket();
		struct sockaddr_in getServerAddress();

		//setters
		void setServerSocket(int server_socket);
		void setServerAddress(struct sockaddr_in server_address);

		//methods
		void createServers();
};

