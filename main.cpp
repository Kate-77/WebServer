#include "conf/Lexer.hpp"
#include "conf/Parser.hpp"
#include "cgi/cgi.hpp"
#include "conf/Access.hpp"
#include <arpa/inet.h>


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

    // const std::vector<Parser *> * _Servers = a.getparserv();

    // const std::vector<Parser *>::iterator & it = _Servers;
    // std::cout << it->getPort() << std::endl;
    CGI b;
    b.execute();

    return 0;
}