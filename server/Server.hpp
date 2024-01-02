#pragma once
#include "Socket.hpp"
#include "Client.hpp"
#include <sys/select.h>
#include <unistd.h>
#include "../conf/Lexer.hpp"
#include "../conf/Parser.hpp"
#include "../request/HttpRequestParser.hpp"


class Socket;
class Client;
class HttpRequestParser;

class Server {
	private:
		fd_set master_read_fds, master_write_fds;
		fd_set tmp_read_fds, tmp_write_fds;
		int fdmax;
		std::vector<std::pair<Client, Parser *> > clients;


	public:
		//canonical form
		Server();
		~Server();
		Server(const Server &server);
		Server& operator=(const Server &server);

		//getters
		fd_set getMasterReadFds();
		fd_set getMasterWriteFds();
		fd_set getTmpReadFds();
		fd_set getTmpWriteFds();
		int getFdmax();
		std::vector<std::pair<Client, Parser *> > getClients();

		//setters
		void setMasterReadFds(fd_set master_read_fds);
		void setMasterWriteFds(fd_set master_write_fds);
		void setTmpReadFds(fd_set tmp_read_fds);
		void setTmpWriteFds(fd_set tmp_write_fds);
		void setFdmax(int fdmax);
		void setClients(std::vector<std::pair<Client, Parser *> > clients);

		//methods
		void handleServers(std::vector<std::pair<Socket, Parser *> > & servers);
		void add_servers(std::vector<std::pair<Socket, Parser *> > & servers, fd_set & master_read_fds);
		void handle_new_connections(std::vector<std::pair<Socket, Parser *> > & servers, fd_set & tmp_read_fds);
		void handle_recv_err(int socket, ssize_t nbytes, int i, fd_set & master_read_fds, fd_set & master_write_fds);
		void parse_req(Client & client, unsigned char * buf, ssize_t nbytes, fd_set & master_read_fds, fd_set & master_write_fds);
		void resp_send(Response & response);
};