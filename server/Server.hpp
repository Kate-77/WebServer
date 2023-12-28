#pragma once
#include "Socket.hpp"
#include "Client.hpp"
#include <sys/select.h>
#include <unistd.h>
#include "../conf/Lexer.hpp"
#include "../conf/Parser.hpp"


class Socket;
class Client;

class Server {
	private:
		fd_set master_read_fds, master_write_fds;
		fd_set tmp_read_fds, tmp_write_fds;
		int fdmax;
		Client client;


	public:
		//canonical form
		Server();
		~Server();
		Server(const Server &server);
		Server& operator=(const Server &server);

		//getters

		//setters

		//methods
		void handleServers(Socket socket);

};