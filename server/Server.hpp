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
		// Client client;
		std::vector<std::pair<Client, Parser *> > clients;


	public:
		//canonical form
		Server();
		~Server();
		Server(const Server &server);
		Server& operator=(const Server &server);

		//getters

		//setters

		//methods
		void handleServers(std::vector<std::pair<Socket, Parser *> > & servers);
		void add_servers(std::vector<std::pair<Socket, Parser *> > & servers, fd_set & master_read_fds);
		void handle_new_connections(std::vector<std::pair<Socket, Parser *> > & servers, fd_set & tmp_read_fds);

};