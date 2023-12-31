#ifndef PARSER_HPP
# define PARSER_HPP


# include <string> //find_first_ not_of substr length  append
# include <vector>
# include <map>
# include "Lexer.hpp"
# include <algorithm> //count
# include <ios> //boolalpha
# include <arpa/inet.h>

const std::string       Default_Root = "/var/www/"; 
const std::string           ClientMaxBodySize = "1M";


typedef struct {
   std::string _address;
   unsigned int _port; //[0-65,535]
} address_port;

class Parser {
  private:
    //token type
    typedef std::vector<std::string> _type;
    //call all the parse functions
    typedef void (Parser::*_typedirective)(_type::const_iterator &); //pointer to a function returning void and  -> parse function
    //ALL directives of server block
    std::map<std::string, _typedirective> _sDirectives;
    //ALL directives of location block
    std::map<std::string, _typedirective> _lDirectives;

    //directives attribues
    in_addr_t                             _host;
		uint16_t                              _port;
    std::string                           _hostnormal;
    unsigned int                          _portnormal;
    std::vector<address_port *>           _listen;
    std::map<std::string, Parser *>       _location; //allows to route request to correct location within the file system. 
    std::vector<std::string>              _server_name; //determine which server block is used for a given request.
    std::string                           _root; //site's document root folder
    std::string                           _alias; // remap url to a different directory other than root location
    std::vector<std::string>              _index; //defines files that will be used as an index
    bool                                  _autoindex; //automatically generate a listing 
    std::map<int, std::string>            _error_page; //defines the URI that will be shown for the specified errors
    std::vector<std::string>              _limit_except; //Limits allowed HTTP methods inside a location.
    std::string                           _client_max_body_size; //sets the max allowed size of the client request body
    std::string                           _upload_store; //Specifies a directory to which output files will be saved to. 
    std::map<int, std::string>            _return; //Stops processing and returns the specified code to a client.
    std::map<std::string, std::string>    _cgi;
    std::map<int, std::string>      _code_status;
    std::string                           _slocation;
    //Copy constructor / inutile
    Parser(const Parser & other); 

    //Copy assignment operator / inutile
    Parser & operator=(const Parser & other);


    //parsing directives
    Parser * copyLocation(void);
    void parseLocation(_type::const_iterator & it);
    bool checkIp(const std::string & ip);     //parse address
    bool checkPort(const std::string & port);     //parse port
    void parseListen(_type::const_iterator & it);
    void parseServerName(_type::const_iterator & it);
    void parseRoot(_type::const_iterator & it);
    void parseIndex(_type::const_iterator & it);
    void parseAutoindex(_type::const_iterator & it);
    void parseErrorPage(_type::const_iterator & it);
    void parseLimitExcept(_type::const_iterator & it);
    void parseClientMaxBodySize(_type::const_iterator & it);
    void parseUploadStore(_type::const_iterator & it);
    void parseAlias(_type::const_iterator & it);
    void parseReturn(_type::const_iterator & it);
    void parseCgi(_type::const_iterator & it);
    void init_code_status();
  public:
    //Constructor
    Parser(void);

    //Destructor
    ~Parser(void);

    ////Debug: print parser
    void printparser(const std::string & str) const;

    //some checking
    void secondparser(_type::const_iterator & it);

    //getters of directives
    std::vector<address_port *> &         getListen(void);
    std::vector<std::string> &            getServerName(void);
    std::map<std::string, Parser *> &     getLocation(void);
    std::map<int, std::string>  &         getstatuscode(void);
    std::string &                         getRoot(void);
    std::string &                         getAlias(void); 
    std::vector<std::string>         &    getIndex(void); 
    bool        &                         getAutoindex(void); 
    std::map<int, std::string>            getError_page(void); 
    std::vector<std::string>              getLimit_except(void); 
    std::string &                         getClient_max_body_size(void); 
    std::string &                         getUpload_store(void); 
    std::map<int, std::string>            getReturn(void); 
    std::map<std::string, std::string>    getCgi(void);
    std::string &                         getslocation(void);
    uint16_t & getPort(void);
		in_addr_t & getHost(void);
    unsigned int  &  getPortnormal();
    std::string & getHostnormal();
    

    //exeception nested class
    class ParserException : public std::exception 
    {
      private:
        std::string _message;
      public:
        ParserException(const std::string & message);
        ~ParserException(void) throw ();
        const char * what(void) const throw ();
    };
};


#endif 
