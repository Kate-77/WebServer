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
			break;
		}

		// handle new connections
		handle_new_connections(servers, tmp_read_fds);
		
		// run through the existing client connections looking for data to read or write
		for(size_t i = 0; i < clients.size(); i++)
		{
			//check for read
			if (FD_ISSET(clients[i].first.getClientSocket(), &tmp_read_fds))
			{
				// //printf("******read start*********\n");
				unsigned char buf[1024];
				bzero(buf, 1024);
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

				// //printf("method = [%s]\n", clients[i].first.request.getMethod().c_str());
				// //printf("path = [%s]\n", clients[i].first.request.getPath().c_str());
				// //printf("query = [%s]\n", clients[i].first.request.getQuery().c_str());
				// //printf("version = [%s]\n", clients[i].first.request.getVersion().c_str());
				// std::map<std::string, std::string> headersMap = clients[i].first.request.getHeadersMap();
				// for (std::map<std::string, std::string>::const_iterator it = headersMap.begin(); it != headersMap.end(); ++it) {
   				// 	//std::cout << it->first << " => " << it->second << '\n';}

				//printf("******read end*********\n");
			}
			// check for write
			// //printf("******write start*********\n");
			if (FD_ISSET(clients[i].first.getClientSocket(), &tmp_write_fds))
			{
				////std::cerr << "YYYYYYYYYYYYY__00" << std::endl;
				clients[i].first.response.client_fd = clients[i].first.getClientSocket();

				if(!clients[i].first.response.res_initialized)
				{
					////std::cerr << "YYYYYYYYYYYYY__01" << std::endl;




					// std::cout << "REQUEST STATUS: " << this->clients[i].first.request.getStatusCode() << std::endl;
					// std::cout << "REQUEST METHOD: " << this->clients[i].first.request.getMethod() << std::endl;
					// std::cout << "REQUEST PATH: " << this->clients[i].first.request.getPath() << std::endl;
					// std::cout << "REQUEST QUERY: " << this->clients[i].first.request.getQuery() << std::endl;
					// std::cout << "REQUEST VERSION: " << this->clients[i].first.request.getVersion() << std::endl;
					// std::cout << "REQUEST HEADERS: " <<  std::endl;
					// std::map<std::string, std::string> headersMap = clients[i].first.request.getHeadersMap();
					// for (std::map<std::string, std::string>::const_iterator it = headersMap.begin(); it != headersMap.end(); ++it) {
   					// 	std::cout << it->first << " => " << it->second << '\n';}
					// std::cout << "REQUEST BODY: " << this->clients[i].first.request.getBodyFileName() << std::endl;






					clients[i].first.response.handleResponse(clients[i].first.request , *clients[i].second);
					////std::cerr << "YYYYYYYYYYYYY__02" << std::endl;



					clients[i].first.response.res_initialized = true;
				}
					////std::cerr << "YYYYYYYYYYYYY__03" << std::endl;

				if (clients[i].first.response.client_done)
				{
					////std::cerr << "YYYYYYYYYYYYY__04" << std::endl;
					//printf("client done & removed from write\n");
					FD_CLR(clients[i].first.getClientSocket(), &master_write_fds);
					////std::cerr << "YYYYYYYYYYYYY__05" << std::endl;
					close(clients[i].first.getClientSocket());
					////std::cerr << "YYYYYYYYYYYYY__06" << std::endl;
					clients[i].first.response._file_fd.close();
					////std::cerr << "YYYYYYYYYYYYY__07" << std::endl;
						// if (clients[i].first.request.getMethod() == "POST") {
							// if (access(clients[i].first.request.bodyFileName.c_str(), F_OK) != -1) {
							// 	remove(clients[i].first.request.bodyFileName.c_str());
							// } else {
							// 	//std::cout << "Error delete" << std::endl;
							// }
						// }
					//clien size
					// //printf("clients size befor = %lu\n", clients.size());
					clients.erase(clients.begin() + i);
					////std::cerr << "YYYYYYYYYYYYY__08" << std::endl;
					// //printf("clients size after = %lu\n", clients.size());
					// //printf("i = %lu\n", i);
					i--;
				}
				else
				{
					////std::cerr << "YYYYYYYYYYYYY__09" << std::endl;
					//send response
					// //printf("sending response\n");
					resp_send(clients[i].first.response, clients[i].first.request);
					////std::cerr << "YYYYYYYYYYYYY__10" << std::endl;
					// resp_send(clients[i].first.response);
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
			//std::cout << "Connection closed by client "  << std::endl;
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
	buf[nbytes] = '\0';

	try {
		printf("***parse request****\n");
		client.request.parseRequest(nbytes, buf, Done);
	}
	catch (int code) {
		printf("***parse request====================================****\n");
 		client.request.setStatusCode(code);
		Done = 1;
	}

	//printf("code = %d\n", client.request.getStatusCode());

	if (Done == 1)
	{
		std::cerr << "XXXXXXXXX__00__" << std::endl;
		// //printf("Req Done _ moved to write\n");
		//move client to write set
		FD_CLR(client.getClientSocket(), &master_read_fds);
		std::cerr << "XXXXXXXXX__01__" << std::endl;
		FD_SET(client.getClientSocket(), &master_write_fds);
		std::cerr << "XXXXXXXXX__02__" << std::endl;
	}
}
// void Server::resp_send(Response &response)
void Server::resp_send(Response &response, HttpRequestParser &req)
{
	// //general response to test
	// std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, world!";

	// if (send(response.client_fd, httpResponse.c_str(), httpResponse.length(), 0) == -1)
	// 	perror("send");

	// response.client_done = true;

	// //send dlm39ol
	// // 1-head
	ssize_t keep_track = 0;
	// //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!sending response\n");
	// //printf("salaaaam \n");
	if(response._head.size() != 0)
    {
		// //printf("sending head: %s\n", response._head.c_str());
        keep_track = send(response.client_fd, response._head.c_str(), response._head.size(), 0) ;

        if(response.sent < 0)
        {
			// //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
            response.client_done = true;
            return;
        }
        response._head = "";
    }
	else if (response._response.size() != 0)
	{
		// //std::cout << response._response.c_str() << std::endl;
		keep_track = send(response.client_fd, response._response.c_str(), response._response.size(), 0);
		if (keep_track <= 0)
		{
			// //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!2\n");
			response.client_done = true;
            return;
		}
		response._response = "";
		response.sent += keep_track;
	}

	else 
	{
		if (!response._file_fd.is_open() && response._file_path.size() != 0)
		{
			response._file_fd.open(response._file_path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
			if (!response._file_fd.is_open())
			{
				// //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!3\n");
				response.client_done = true;
				return;
			}
			response._file_fd.seekg(response.sent, std::ios::beg);
		}

		char buf[1024];
		bzero(buf, 1024);
		response._file_fd.read(buf, 1024);
		size_t buffer_size = response._file_fd.gcount();
		if (buffer_size)
		{
			keep_track = send(response.client_fd, buf, buffer_size, 0);
			if (keep_track <= 0)
			{
				// //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!4\n");
				response.client_done = true;
				return;
			}
			response.sent += keep_track;
			bzero((buf), 1024);
		}
		response._file_fd.close();
	}
	 //std::cout << "sent:  " << response.sent << " constent length: " << response._contentLength << std::endl;
	if (response.sent == response._contentLength)
	{
		//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!5\n");
		response.client_done = true;
		return;
	}
	if (req.getMethod() != "GET" && !response._contentLength)
	{
		// //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!6\n");
    	response.client_done = true;
		return;
	}
}



