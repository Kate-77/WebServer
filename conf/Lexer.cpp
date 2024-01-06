#include "Lexer.hpp"
#include "Parser.hpp"


Lexer::Lexer(void) 
{
  return ;
}

Lexer::~Lexer(void) 
{
  for (std::vector<Parser *>::iterator it = this->_parserv.begin(); it != this->_parserv.end();) {
    delete *it;
    it = this->_parserv.erase(it);
  }
  return ;
}

//Debug: print before lexer and after parsing
void Lexer::printdebug(void) 
{
  //print before lexer
  for (std::vector<std::string>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); ++it) 
  {
    std::cout << *it << std::endl;
  }

  //print after lexer
  for (std::vector<Parser *>::const_iterator it = this->_parserv.begin(); it != this->_parserv.end(); ++it) 
  {
    std::cout << *it << std::endl;
  }
}

void Lexer::start(const char * configfile) 
{
  if(configfile)
    this->_configfile = configfile;
  else
    this->_configfile = "default.conf";    
  this->start_lexer();
  // this->printdebug();
  this->preparsing();

  return ;
}

const std::vector<Parser *> * Lexer::getparserv(void) const 
{
  return (&(this->_parserv));
}

//tokenize
void Lexer::start_lexer(void) 
{
  std::ifstream   configfile; // internal file stream
  bool            commentaire;

  //open file for reading
  configfile.open(this->_configfile, std::ifstream::in);
  if (!configfile.is_open()) 
  {
    throw Lexer::LexerException("Error! Failed to open: " + std::string(this->_configfile));
  }

  //check configfile line by line
  for (std::string line; getline(configfile, line);) 
  {
    commentaire = false;
    //to find the start of the line we use:
    //find_first_not_of: Searches the string for the first character that does not match any of the characters specified in its arguments.
    //If no such characters are found, the function returns string::npos.
    //pos:Position of the first character in the string to be considered in the search.
    std::string::size_type  start = line.find_first_not_of(" \t", 0);
    std::string::size_type  end = 0;

    //process the line segment by segment
    //npos: until the end of the string"
    while ((start = line.find_first_not_of(" \t", end)) != std::string::npos) 
    {
        end = line.find_first_of(" \t", start);

        //fill the string with the line 
        std::string token = line.substr(start, end - start);

        // check the comments, if true we break
        if ('#' == token[0]) 
        {
          commentaire = true;
          break ;
        }

        // insert token and check semicolon
        if (token.length() > 1 && token[token.length() - 1] == ';') 
        {
          token.erase(token.end() - 1);
          this->_tokens.push_back(token);
          this->_tokens.push_back(";");
        }
        else 
        {
          this->_tokens.push_back(token);
        }
    }
    if (commentaire == true) 
    {
      continue ;
    }
  }
  // close file
  configfile.close();
}

// parse tokens
void Lexer::preparsing(void) 
{
  int bracescount = 0;

  // check braces for every token
  for (_type::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); ++it) 
  {
    // check braces for every char of token
    std::string str(it->begin(), it->end());
    for (unsigned long i  = 0; i < str.length(); ++i)
    {
      if (str[i] == *"{") 
      {
        ++bracescount;
      } 
      else if (str[i] == *"}") 
      {
        if (bracescount == 0) 
        {
            throw Lexer::LexerException("Error! extra closing brace on configfile");
        }
        --bracescount;
      }
    }
  }
  if (bracescount > 0) 
  {
    throw Lexer::LexerException("Error! missing brace on configfile");
  }

  // check server block
  int servercount = 0;
  for (_type::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); ++it) 
  {
    if (*it == "server") 
    {
      Parser * begin = new Parser();
      try 
      {
        begin->secondparser(++it);
      } 
      catch (std::exception & e) 
      {
        delete begin;
        std::cerr << e.what() << std::endl;
        throw Lexer::LexerException("Error! server block parser throw an exception");
      }
      this->_parserv.push_back(begin);
      ++servercount;
    } 
    else 
    {
      throw Lexer::LexerException("Error! unknown directive: " + *it);
    }
  }
  if (servercount == 0) 
  {
    throw Lexer::LexerException("Error! missing 'server' block");
  }
  for(std::vector<Parser *>::const_iterator it = this->_parserv.begin(); it != this->_parserv.end(); it++)
  {
      // std::vector<address_port *>  b = (*it)->getListen();
      // std::vector<address_port *>::const_iterator c = b.begin();
      // std::string ip = (*c)->_address;
      // unsigned int port = (*c)->_port;
      // // std::cout << "first " << ip << ":" <<port << std::endl;
      // for(std::vector<Parser *>::const_iterator its = this->_parserv.begin(); its != it; its++)
      // {
      //   std::vector<address_port *> ip_port = (*its)->getListen();
      //   for (std::vector<address_port *>::const_iterator itl = ip_port.begin(); itl != ip_port.end(); ++itl) 
      //   {
      //   // std::cout << "second " << (*itl)->_address << ":" <<(*itl)->_port << std::endl;
      //     if((*itl)->_address == ip && (*itl)->_port == port)
      //       throw Parser::ParserException("Error! duplicate value in directive 'listen'");
      //   }
      // }
      unsigned int port = (*it)->getPortnormal();
      std::string host = (*it)->getHostnormal();
      std::vector<std::string>::const_iterator iname = (*it)->getServerName().begin();
      std::string name = (*iname);
    for(std::vector<Parser *>::const_iterator its = this->_parserv.begin(); its != it; its++)
    {
      unsigned int ports = (*its)->getPortnormal();
      std::string hosts = (*its)->getHostnormal();
      std::vector<std::string>::const_iterator inames = (*its)->getServerName().begin();
       std::string names = (*inames);
      if(port == ports && host == hosts && name == names)
        throw Parser::ParserException("Error! duplicate value in directive 'listen' and directive 'server name'");
    }
  }
  return ;
}

Lexer::LexerException::LexerException(const std::string & message) : _message(message) 
{
  return ;
}
Lexer::LexerException::~LexerException(void) throw () 
{
  return ;
}
const char * Lexer::LexerException::what(void) const throw () 
{
  return this->_message.c_str();
}

