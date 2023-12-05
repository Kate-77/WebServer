#ifndef Lexer_HPP
# define Lexer_HPP

# include <string>
# include <vector>

class Parser;
//class ClientServer;
struct address_port {
   std::string _address;
   unsigned int _port; //[0-65,535]
};

class Lexer {
  private:
    //token type
    typedef std::vector<std::string> _type;

    //convert configuration file into tokens
    std::vector<std::string>      _tokens; 

    //convert tokens to vector to parse it
    std::vector<Parser *>   _parserv; 

    //configuration file
    const char *                  _file;

    //Copy constructor / inutile
    Lexer( const Lexer & other ); 

    //Copy assignment operator / inutile
    Lexer &operator=( const Lexer& other );

    void        Lexer( void ); 
    void        preparsing( void );  //_tokens to _parsev


  public:
    //Constructor
    Lexer( void );

    //Destructor
    virtual ~Lexer( void );

    //Debug: print before lexer and after parsing
    void printdebug( void ); 

    //the main and the beginning
    void start( const char* file );

    //getter
    const std::vector<Parser *> * getparserv( void ) const;

    //exeception nested class
    class LexerException : public std::exception {
      private:
        std::string _message;
      public:
       LexerException( const std::string & message );
        virtual ~LexerException( void ) throw ();
        virtual const char * what( void ) const throw ();
    };
};


#endif 
