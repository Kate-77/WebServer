#pragma once
#include <netinet/in.h>
#include <cstring>
#include "../request/HttpRequestParser.hpp"

class HttpRequestParser;

class Client {
	private:
		int client_socket;
		struct sockaddr_in client_address;
	public:
		HttpRequestParser request;

		//canonical form
		Client();
		~Client();
		Client(const Client &client);
		Client& operator=(const Client &client);
		

		//getters
		int getClientSocket();
		struct sockaddr_in getClientAddress();
		HttpRequestParser getRequest();
		

		//setters
		void setClientSocket(int client_socket);
		void setClientAddress(struct sockaddr_in client_address);
		void setRequest(HttpRequestParser request);

		//methods
		
};