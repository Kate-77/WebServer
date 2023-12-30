// #ifndef Access_HPP
// # define Access_HPP
// # include "Parser.hpp"
// # include "Lexer.hpp"
// # include <iostream>
// # include <arpa/inet.h>
// //class for get directives from a server
// class Access {
//     private:
// 		in_addr_t _host;
// 		uint16_t _port;
//     public:
//         void start(std::vector<Parser *>::const_iterator it);
//         uint16_t & getPort(void);
// 		in_addr_t & getHost(void);
    
//     // std::string _root;
//     // std::string _server_name;
// 	// 	unsigned long _client_max_body_size;
// 	// 	std::string _index;
// 	// 	bool _autoindex;
// 	// 	std::vector<Location_storage> _locations;
// 	// 	std::map<int, std::string> _error_pages;
//     //     int _listen_fd;
//     //     struct sockaddr_in _server_address;

//     // location
//     // std::string loca_path;
// 	// 	std::string loca_root;
// 	// 	bool 		loca_autoindex;
// 	// 	std::string	loca_index;
// 	// 	storage_int loca_allowed_methods;
// 	// 	std::string	loca_redirect;
// 	// 	std::string	loca_alias;
// 	// 	bool 		loca_upload;
// 	// 	storage		loca_cgi_path;
// 	// 	// stotage	loca_cgi_extension;
// 	// 	unsigned long loca_client_max_body_size;
// };

// #endif