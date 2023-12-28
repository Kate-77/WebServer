#ifndef Lexer_HPP
# define Lexer_HPP

# include <string>
# include <vector>
# include <iostream> //std::cout , std::cerr, std::endl
#include <fstream> //std::ifstream
# include "Parser.hpp"

class Parser;
//class ClientServer;

class Lexer {
  private:
    //token type
    typedef std::vector<std::string> _type;

    //convert configuration file into tokens
    std::vector<std::string>        _tokens; 

    //convert tokens to vector to parse it
    std::vector<Parser *>          _parserv; 

    //configuration file
    const char *                  _configfile;

    //Copy constructor / inutile
    Lexer(const Lexer & obj); 

    //Copy assignment operator / inutile
    Lexer &operator=(const Lexer & rhs);

    void        start_lexer(void); 
    void        preparsing(void);  //_tokens to _parsev


  public:
    //Constructor
    Lexer(void);

    //Destructor
    virtual ~Lexer(void);

    //Debug: print before lexer and after parsing
    void printdebug(void); 

    //the main and the beginning
    void start(const char* file);

    //getter
    const std::vector<Parser *> * getparserv(void) const;

    //exeception nested class
    class LexerException : public std::exception 
    {
      private:
        std::string _message;
      public:
       LexerException(const std::string & message);
       ~LexerException(void) throw ();
       const char * what(void) const throw ();
    };
};


#endif 
