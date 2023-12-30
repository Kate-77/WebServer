#ifndef Access_HPP
# define Access_HPP
# include "Parser.hpp"
# include "Lexer.hpp"
# include <iostream>
class Access {
    private:
    Lexer *              _Lexer; //ici define inside the classs
    const std::vector<Parser *> * _Servers; // icidefine inside the class
		// in_addr_t _host;
		// std::string _root;
    public:
    void start(void);
    // std::string _server_name;
	// 	uint16_t _port;
	// 	unsigned long _client_max_body_size;
	// 	std::string _index;
	// 	bool _autoindex;
	// 	std::vector<Location_storage> _locations;
	// 	std::map<int, std::string> _error_pages;
    //     int _listen_fd;
    //     struct sockaddr_in _server_address;

    // location
    // std::string loca_path;
	// 	std::string loca_root;
	// 	bool 		loca_autoindex;
	// 	std::string	loca_index;
	// 	storage_int loca_allowed_methods;
	// 	std::string	loca_redirect;
	// 	std::string	loca_alias;
	// 	bool 		loca_upload;
	// 	storage		loca_cgi_path;
	// 	// stotage	loca_cgi_extension;
	// 	unsigned long loca_client_max_body_size;
};

#endif