#include "conf/Lexer.hpp"
#include "conf/Parser.hpp"
#include "cgi/cgi.hpp"
#include "server/Server.hpp"
#include "server/Socket.hpp"
#include "server/Client.hpp"

int main()
{
    Socket socket;
	Server server;

	socket.createServers();
	server.handleServers(socket);
    return 0;
}