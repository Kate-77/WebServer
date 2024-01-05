#include "conf/Lexer.hpp"
#include "conf/Parser.hpp"
#include "cgi/cgi.hpp"
#include "server/Server.hpp"
#include "server/Socket.hpp"
#include "server/Client.hpp"

int main(int ac, char **av)
{
 	Lexer a;
    if (ac > 2)
    {
        std::cerr << "Error!" << std::endl;
    }
    try
    {
        if(av[1])
             a.start(av[1]);
        else
             a.start(NULL);
    }
    catch(std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

    Socket socket;
	Server server;
	const std::vector<Parser *> *servers = a.getparserv();
	std::vector<std::pair<Socket, Parser *> >  final_servers;

	socket.createServers(servers, final_servers);
	
	server.handleServers(final_servers);
    return 0;
}