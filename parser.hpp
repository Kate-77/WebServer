#ifndef PARSER_HPP
# define PARSER_HPP


# include <string>
# include <vector>
# include <map>
# include "lexer.hpp"

struct Listen;

class Parser {
  private:
    //token type
    typedef std::vector<std::string> _type;

    //ALL directives of server block
    std::map<std::string, _type::const_iterator &> _sDirectives;
    //ALL directives of location block
    std::map<std::string, _type::const_iterator &> _lDirectives;

    //directives attribues
    std::vector<Listen *>         _listen; 
    std::map<std::string, Parser *> _location; //allows to route request to correct location within the file system. 
    std::vector<std::string>      _server_name; //determine which server block is used for a given request.
    std::string             _root; //site's document root folder
    std::string             _alias; // remap url to a different directory other than root location
    std::vector<std::string>      _index; //defines files that will be used as an index
    bool                _autoindex; //automatically generate a listing 
    std::map<int, std::string>      _error_page; //defines the URI that will be shown for the specified errors
    std::vector<std::string>      _limit_except; //Limits allowed HTTP methods inside a location.
    std::string::size_type        _client_max_body_size; //sets the max allowed size of the client request body
    std::string             _upload_store; //Specifies a directory to which output files will be saved to. 
    std::pair<int, std::string>     _return; //Stops processing and returns the specified code to a client.
    std::map<std::string, std::string>  _cgi;

    //Copy constructor / inutile
    Parser( const Parser & other ); 

    //Copy assignment operator / inutile
    Parser & operator=( const Parser & other );

    //location parser reminder

    //parsing directives
    void parseLocation(       _type::const_iterator & it );
    void parseListen(       _type::const_iterator & it );
    void parseServerName(     _type::const_iterator & it );
    void parseRoot(         _type::const_iterator & it );
    void parseIndex(        _type::const_iterator & it );
    void parseAutoindex(      _type::const_iterator & it );
    void parseErrorPage(      _type::const_iterator & it );
    void parseLimitExcept(      _type::const_iterator & it );
    void parseClientMaxBodySize(  _type::const_iterator & it );
    void parseUploadStore(      _type::const_iterator & it );
    void parseAlias(        _type::const_iterator & it );
    void parseReturn(       _type::const_iterator & it );
    void parseCgi(          _type::const_iterator & it );


  public:
    //Constructor
    Parser( void );

    //Destructor
    ~Parser( void );

    ////Debug: print parser
    void printparser( const std::string & indent ) const;

    //some checking
    void secondparser( _type::const_iterator & it );

    //getters of directives
    std::vector<Listen *> &         getListen( void );
    std::vector<std::string> &        getServerName( void );
    std::map<std::string, Parser *> & getLocation( void );
    

    //exeception nested class
    class ParserException : virtual public std::exception {
      private:
        std::string _message;
      public:
        ParserException( const std::string & message );
        virtual ~ParserException( void ) throw ();
        virtual const char * what( void ) const throw ();
    };
};


#endif 
