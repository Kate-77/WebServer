#include "servers.hpp"

/*
 * This Function  create sockets for servers, set them to non-block mood,
 * configure the port, bind them and listen
 */

/*
 * getClientResponse() function :***********************************************************
 */




/*
 * This Function  create sockets for servers, set them to non-block mood,
 * configure the port, bind them and listen
 */

int		create_servers( std::vector< Server > &servers ) {
	std::vector<int>    bound;
	for (Servers::iterator it = servers.begin(); it != servers.end(); it++) {
		// Create Socket
		if (it->get_socketInfos().createSocket() == -1) {
			perror("Socket");
			return (-1);
		}
		fcntl(it->get_socketInfos().createSocket(), F_SETFL, O_NONBLOCK);
		// set Socket Address
		it->get_socketInfos().setSocketAddress(it->get_port(), it->get_host());

		// test if the socket is already bound to prevent bind failure
		if (find(bound.begin(), bound.end(), it->get_port()) == bound.end()) {
			// Bind Socket if not already bound
			if(it->get_socketInfos().bindSocket() == -1) {
				perror("Bind");
				return (-1);
			}
			else {
				bound.push_back(it->get_port());
			}
		}

		// Listen on socket
		if (it->get_socketInfos().listenSocket() == -1) {
			perror("Listen");
			return (-1);
		}
	// }
	return (0);
}

/*
 * This Function add all lunched servers to the select read_set
 */




/*
 * Accept a connection on a server add new client
 * and mapped request to the read_client vector
 */




/*
 * loop over all servers fds after select return fds are ready for read
 * to see which fd is ready for accepting connection using FD_ISSET
 */

void	accept_connections( Servers &servers, ClientRequest &read_clients, fd_set &backup_rset ) {

	// for (Servers::iterator it = servers.begin(); it != servers.end(); it++) {
	// 	if (FD_ISSET(it->get_socketInfos().getSocketFd(), &backup_rset) ){
	// 		accept_connection(read_clients, it->get_socketInfos().getSocketFd());
	// 	}
	// }
}




void	handle_all_servers( std::vector<Server> &servers, fd_set &read_fds, fd_set &write_fds, int &maxfd )
{
	ClientRequest	read_clients;
	ClientResponse	write_clients;

	// create another set bcs select is destroys the set feeded
	fd_set	backup_rset, backup_wset;
	unsigned int status;
	while (42)
	{
		// initialize fd set
		backup_rset = read_fds;
		backup_wset = write_fds;

		// add reading client socket file descriptors to rset
		add_read_clients(read_clients, backup_rset, maxfd);
		// add reading client socket file descriptors to rset
		add_write_clients(write_clients, backup_wset, maxfd);
		// feed fds to select only for read option
		status = select(maxfd + 1, &backup_rset, &backup_wset, NULL, NULL);
		// protection for select
		if (status < 0)
		{
			std::cerr << "Select Failed!" << std::endl;
		}
		// if the fd is between servers fds accept new connection
		accept_connections(servers, read_clients, backup_rset);
		// if the fd is between read_clients fds accept read request
		handle_clients_requests(read_clients, write_clients, backup_rset, servers);
		// if the fd is between write_client fds write response
		handle_clients_responses(write_clients, read_clients, backup_wset);
	}
}