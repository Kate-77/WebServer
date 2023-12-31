
#include "Lexer.hpp"
#include "Parser.hpp"



Parser::Parser(void) : _root(Default_Root), _autoindex(false), _client_max_body_size(ClientMaxBodySize), _return() 
{
  //server block
  //the second iterator is a pointer to function of return type void and  vector iterator parametre
  this->_sDirectives["location"]       = &Parser::parseLocation; //done
  this->_sDirectives["listen"]       = &Parser::parseListen;    //done
  this->_sDirectives["server_name"]      = &Parser::parseServerName;  //done
  this->_sDirectives["root"]         = &Parser::parseRoot;
  this->_sDirectives["index"]        = &Parser::parseIndex;       //done
  this->_sDirectives["autoindex"]      = &Parser::parseAutoindex;     //done
  this->_sDirectives["error_page"]     = &Parser::parseErrorPage;             //DONE
  this->_sDirectives["limit_except"]     = &Parser::parseLimitExcept;         //DONE
  this->_sDirectives["client_max_body_size"] = &Parser::parseClientMaxBodySize; //done
  this->_sDirectives["upload_store"]     = &Parser::parseUploadStore; //DONE
  this->_sDirectives["cgi"]          = &Parser::parseCgi;           //done
  this->_sDirectives["return"]         = &Parser::parseReturn; //done
  //location block
  //the second iterator is a pointer to function of return type void and  vector iterator parametre
  this->_lDirectives["alias"]          = &Parser::parseAlias; 
  this->_lDirectives["return"]         = &Parser::parseReturn; //done
  this->_lDirectives["root"]         = &Parser::parseRoot;    //done
  this->_lDirectives["index"]          = &Parser::parseIndex;       //done
  this->_lDirectives["autoindex"]        = &Parser::parseAutoindex;     //done
  this->_lDirectives["error_page"]       = &Parser::parseErrorPage; //done
  this->_lDirectives["limit_except"]     = &Parser::parseLimitExcept;     //DONE
  this->_lDirectives["client_max_body_size"] = &Parser::parseClientMaxBodySize; //done
  this->_lDirectives["upload_store"]     = &Parser::parseUploadStore;
  this->_lDirectives["cgi"]          = &Parser::parseCgi;             //done
  this->init_code_status();
}

Parser::~Parser(void) 
{
  for (std::vector<address_port *>::iterator it = this->_listen.begin(); it != this->_listen.end();) 
  {
    delete *it;
    it = this->_listen.erase(it);
  }
  for (std::map<std::string, Parser *>::iterator it = this->_location.begin(); it != this->_location.end();) 
  {
    delete it->second;
    this->_location.erase(it++);
  }
  return ;
}

void Parser::init_code_status()
{
  //1xx information response
  this->_code_status[100] = "Continue";
  this->_code_status[101] = "Switching Protocols";
  this->_code_status[102] = "Processing";
  this->_code_status[103] = "Early Hints";
  //2xx   Successful resposes
  this->_code_status[200] = "OK";
  this->_code_status[201] = "Created";
  this->_code_status[202] = "Accepted";
  this->_code_status[203] = "Non-Authoritative Information";
  this->_code_status[204] = "No Content";
  this->_code_status[205] = "Reset Content";
  this->_code_status[206] = "Partial Content";
  this->_code_status[207] = "Multi-Status";
  this->_code_status[208] = "Already Reported";
  this->_code_status[226] = "IM USED";
  //3xx Redirection messages
  this->_code_status[300] = "Multiple Choices";
  this->_code_status[301] = "Moved Permanently";
  this->_code_status[302] = "Found";
  this->_code_status[303] = "See Other";
  this->_code_status[304] = "Not Modified";
  this->_code_status[305] = "Use Proxy "; //It has been deprecated due to security concerns regarding in-band configuration of a proxy.
  this->_code_status[306] = "unused"; //This response code is no longer used; it is just reserved.
  this->_code_status[307] = "Temporary Redirect";
  this->_code_status[308] = "Permanent Redirect";
  //4xx Client error responses
  this->_code_status[400] = "Bad Request";
  this->_code_status[401] = "Unauthorized";
  this->_code_status[402] = "Payment Required";
  this->_code_status[403] = "Forbidden";
  this->_code_status[404] = "Not Found";
  this->_code_status[405] = "Method Not Allowed";
  this->_code_status[406] = "Not Acceptable";
  this->_code_status[407] = "Proxy Authentication Required";
  this->_code_status[408] = "Request Timeout";
  this->_code_status[409] = "Conflict";
  this->_code_status[410] = "Gone";
  this->_code_status[411] = "Length Required";
  this->_code_status[412] = "Precondition Failed";
  this->_code_status[413] = "Payload Too Large";
  this->_code_status[414] = "URI Too Long";
  this->_code_status[415] = "Unsupported Media Type";
  this->_code_status[416] = "Range Not Satisfiable";
  this->_code_status[417] = "Expectation Failed";
  this->_code_status[418] = "I'm a teapot";
  this->_code_status[421] = "Misdirected Request";
  this->_code_status[422] = "Unprocessable Content";
  this->_code_status[423] = "Locked ";
  this->_code_status[424] = "Failed Dependency";
  this->_code_status[425] = "Too Early";
  this->_code_status[426] = "Upgrade Required";
  this->_code_status[428] = "Precondition Required";
  this->_code_status[429] = "Too Many Requests";
  this->_code_status[431] = "Request Header Fields Too Large";
  this->_code_status[451] = "Unavailable For Legal Reasons";
  //5xx Server error responses
  this->_code_status[500] = "Internal Server Error";
  this->_code_status[501] = "Not Implemented";
  this->_code_status[502] = "Bad Gateway";
  this->_code_status[503] = "Service Unavailable";
  this->_code_status[504] = "Gateway Timeout";
  this->_code_status[505] = "HTTP Version Not Supported";
  this->_code_status[506] = "Variant Also Negotiates";
  this->_code_status[507] = "Insufficient Storage";
  this->_code_status[508] = "Loop Detected";
  this->_code_status[510] = "Not Extended";
  this->_code_status[511] = "Network Authentication Required";
}

void Parser::printparser(const std::string & str) const 
{
  std::cout << std::endl <<  str << std::endl;

  std::cout << "listen:" << std::endl;
  for (std::vector<address_port *>::const_iterator it = this->_listen.begin(); it != this->_listen.end(); ++it) 
  {
    std::cout << "    " << "address: " << (*it)->_address << " port: " << (*it)->_port << std::endl;
  }

  std::cout << "server_name:" << std::endl;
  for (std::vector<std::string>::const_iterator it = this->_server_name.begin(); it != this->_server_name.end(); ++it) 
  {
    std::cout << "    " << *it << std::endl;
  }

  std::cout << "root:  " << this->_root << std::endl;
  std::cout << "alias: " << this->_alias << std::endl;

  std::cout << "index:" << std::endl;
  for (std::vector<std::string>::const_iterator it = this->_index.begin(); it != this->_index.end(); ++it) 
  {
    std::cout << "    " << *it << std::endl;
  }

  //boolalpha: either true or false, instead of integral values.
  std::cout << "autoindex: " << std::boolalpha << this->_autoindex << std::endl;

  std::cout << "error_page:" << std::endl;
  for (std::map<int, std::string>::const_iterator it = this->_error_page.begin(); it != this->_error_page.end(); ++it) 
  {
    std::cout << "    " << it->first << " " << it->second << std::endl;
  }

  std::cout << "limit_except:" << std::endl;
  for (std::vector<std::string>::const_iterator it = this->_limit_except.begin(); it != this->_limit_except.end(); ++it) 
  {
    std::cout << "    " << *it << std::endl;
  }

  std::cout << "client_max_body_size: " << this->_client_max_body_size << std::endl;
  std::cout << "upload_store:         " << this->_upload_store << std::endl;

  std::cout << "return:" << std::endl;
 for (std::map<int, std::string>::const_iterator it = this->_return.begin(); it != this->_return.end(); ++it) 
  {
    std::cout << "    " << it->first << " " << it->second << std::endl;
  }

  std::cout << "cgi:" << std::endl;
  for (std::map<std::string, std::string>::const_iterator it = this->_cgi.begin(); it != this->_cgi.end(); ++it) 
  {
    std::cout << "    " << it->first << " -> " << it->second << std::endl;
  }

  if (this->_location.empty() == false) 
  {
    std::cout << "locations:" << std::endl;
    for (std::map<std::string, Parser *>::const_iterator it = this->_location.begin(); it != this->_location.end(); ++it)
    {
      std::cout << "    " << it->first << std::endl;
      std::cout << "string location " << it->second->getslocation() << std::endl;
      it->second->printparser("----Location Block----");
    }
  }
}

void Parser::secondparser(_type::const_iterator & it) 
{

  // check opening brace on  server block
  if ("{" != *it)
  {
    throw Parser::ParserException("Error! missing an opening brace on server block");
  }
  ++it;

  // check closing brace
  std::map<std::string, _typedirective>::const_iterator sDirectivesit;
  for (;"}" != *it; ++it) 
  {
    sDirectivesit = this->_sDirectives.find(*it);
    //check if directives exist
    if (this->_sDirectives.end() != sDirectivesit) 
    {
      // check value of directive
      const _type::const_iterator previous = it;
      ++it;
      if ("}" == *it) 
      {
        throw Parser::ParserException("Error! missing directive '" + *previous + "' value in 'server' block");
      }
      //DEBUUUUUUUUG
      //pass it to the parser function -> check constructor
      (this->*(sDirectivesit->second))(it);
    } 
    else 
    {
      throw Parser::ParserException("Error! unknown directive: '" + *it + "' in 'server' block");
    }
  }  
  this->printparser("----Server Block----");

  return ;
}

std::vector<address_port *> & Parser::getListen(void) 
{
  return (this->_listen);
}

std::vector<std::string> & Parser::getServerName(void) 
{
  return (this->_server_name);
}

std::map<std::string, Parser *> & Parser::getLocation(void) 
{
  return (this->_location);
}

std::map<int, std::string>  &         Parser::getstatuscode(void)
{
  return (this->_code_status);
}

std::string &                         Parser::getRoot(void)
{
      return (this->_root);
}

std::string &                         Parser::getAlias(void)
{
      return (this->_alias);
} 

std::vector<std::string>         &    Parser::getIndex(void)
{
      return (this->_index);
} 

bool        &                         Parser::getAutoindex(void)
{
      return (this->_autoindex);
} 

std::map<int, std::string>            Parser::getError_page(void)
{
      return (this->_error_page);
} 

std::vector<std::string>              Parser::getLimit_except(void)
{
      return (this->_limit_except);
} 

std::string &                         Parser::getClient_max_body_size(void)
{
      return (this->_client_max_body_size);
} 

std::string &                         Parser::getUpload_store(void)
{
  return (this->_upload_store);
      
} 

std::map<int, std::string>            Parser::getReturn(void)
{
      return (this->_return);
} 

std::map<std::string, std::string>    Parser::getCgi(void)
{
      return (this->_cgi);
}

uint16_t  & Parser::getPort(void)
{
  return this->_port;
}

in_addr_t & Parser::getHost(void)
{
  return this->_host;
}

unsigned int  & Parser::getPortnormal()
{
  return this->_portnormal;
}

 std::string & Parser::getHostnormal()
{
  return this->_hostnormal;
}

std::string & Parser::getslocation()
{
  return this->_slocation;
}

Parser * Parser::copyLocation(void) 
{
  Parser * location = new Parser();

  location->_root                     = this->_root; // replaced 
  location->_index                    = this->_index; // replaced 
  location->_autoindex                = this->_autoindex; // replaced 
  location->_error_page               = this->_error_page; // added 
  location->_limit_except             = this->_limit_except; // replaced 
  location->_client_max_body_size     = this->_client_max_body_size; // replaced 
  return location;
}

void Parser::parseLocation(_type::const_iterator & it) 
{
  std::string     location    = *it;
  Parser *        copy  = nullptr;

  // check duplicate
  if (this->_location.find(location) != this->_location.end()) 
  {
    throw Parser::ParserException("Error! duplicate value in  'location' block ");
  }
  //uri
  if(it->find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%\\") == std::string::npos)
    it++;
  // check opening  brace
  if ("{" != *it) 
  {
    throw Parser::ParserException("Error! missing opening brace in  'location' block");
  }
  ++it;

  try 
  {
    // create a copy 
    copy = this->copyLocation();

    // check closing brace
    std::map<std::string, _typedirective>::const_iterator lDirectivesit;
    for (;"}" != *it; ++it) 
    {
      lDirectivesit = this->_lDirectives.find(*it);
       //check if directives exist
      if (this->_lDirectives.end() != lDirectivesit)
       {
        // check value of directive
        const _type::const_iterator previous = it;
        ++it;
        if ("}" == *it) 
        {
          throw Parser::ParserException("Error! missing directive '" + *previous + "' value in 'location' block");
        }

        //pass it to the parser function -> check constructor
        (copy->*(lDirectivesit->second))(it);
      } 
      else 
      {
        throw Parser::ParserException("Error! unknown directive: '" + *it + "' in 'location' block");
      }

    }
  } 
  catch (std::exception & e) 
  {
    delete copy;
    std::cerr << e.what() << std::endl;
    throw Parser::ParserException("Error! location block");
  }

  // add location value
  this->_location[location] = copy;
  //save string 
  this->_location[location]->_slocation = location;
  return ;
}



bool Parser::checkIp(const std::string & ip) 
{
  if(ip == "localhost" || ip == "*")
    return (true);
  // check digits and dots
  if (ip.find_first_not_of(".0123456789") != std::string::npos) 
  {
    return (false);
  }
  // check 3 dots
  if (std::count(ip.begin(), ip.end(), '.') != 3) 
  {
    return (false);
  }
  //check number min and format 
  //sscanf: reads the data from null-terminated character string buffer. Reaching the end of the string is equivalent to reaching the end-of-file condition for fscanf
  //u: unsigned int number 
  unsigned int numbers[4];
  if (sscanf(ip.c_str(),"%u.%u.%u.%u", &(numbers[0]), &(numbers[1]), &(numbers[2]), &(numbers[3])) != 4) 
  {
    return (false);
  }
  // check  max digit
  for (int i = 0; i < 4; ++i) 
  {
    if (numbers[i] > 255) {
      return (false);
    }
  }
  return (true);
}

bool Parser::checkPort(const std::string & port) 
{

  // check digits
  if (port.find_first_not_of("0123456789") != std::string::npos) 
  {
    return (false);
  }
  // check is not full of 0
  std::string::size_type start = port.find_first_not_of("0");
  if (start == std::string::npos) 
  {
    return (false);
  }
  // check lenght min: 1, max:65535
  std::string number = port.substr(start);
  if (number.length() < 1 || number.length() > 5) 
  {
    return (false);
  }
  if (number.length() == 5 && number > "65535") 
  {
    return (false);
  }
  return (true);
}

void Parser::parseListen(_type::const_iterator & it) 
{
  std::string   ip = "0.0.0.0"; // default
  unsigned int  port = 80; //default
  std::string             token = *it;
  std::string::size_type  seperator   = token.find(":");

  //check directive value 
  //format:ip:port;
  if (seperator != std::string::npos) 
  {
    std::string temporaireaddress = token.substr(0, seperator);
    std::string temporaireport    = token.substr(seperator + 1);

    if (checkIp(temporaireaddress) == false) 
    {
      throw Parser::ParserException("Error! invalid port on 'listen' directive");
    }
    if (checkPort(temporaireport) == false) 
    {
      throw Parser::ParserException("Error! invalid IP on 'listen' directive");
    }
    ip    = temporaireaddress;
    if(ip == "localhost")
      ip = "127.0.0.1";
    if(ip == "*")
      ip = "0.0.0.0";
    port  = static_cast<unsigned int>(atoi(temporaireport.c_str()));
  }
  //format: ip;
  else if (checkIp(*it) == true) 
  {
    ip = *it;
    if(ip == "localhost")
      ip = "127.0.0.1";
    if(ip == "*")
      ip = "0.0.0.0";
  }
  //format: port;
  else if (checkPort(*it) == true) 
  {
    port = static_cast<unsigned int>(atoi((*it).c_str()));
  } 
  else  
  {
    throw Parser::ParserException("Error! invalid value on 'listen' directive");
  }
  address_port * ip_port = new address_port();
  ip_port->_address = "0";
  ip_port->_port = 1000000;
  // check value not duplicated
  // if (std::find(this->_listen.begin(), this-> _listen.end(), ip_port) != this->_listen.end()) 
  // for (std::vector<address_port *>::const_iterator it = this->_listen.begin(); it != this->_listen.end(); ++it) 
  // {
  //   if((*it)->_address == ip_port->_address && (*it)->_port == ip_port->_port)
  //     throw Parser::ParserException("Error! duplicate value in directive 'listen'");
  // }
  for (std::vector<address_port *>::const_iterator it = this->_listen.begin(); it != this->_listen.end(); ++it) 
  {
    if((*it)->_address != "0" && (*it)->_port != 1000000 )
      throw Parser::ParserException("Error! too many directives 'listen'");
  }
  ip_port->_address = ip;
  ip_port->_port = port;
  this->_portnormal = port;
  this->_hostnormal = ip;
  this->_host = inet_addr(ip.c_str());
  this->_port = htons(port);
  // save
  this->_listen.push_back(ip_port);
  // check semicolon
  ++it;
  //inutile
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near token 'listen'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'listen'");
  }
  return ;
}

void Parser::parseServerName(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'server_name'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! missing values in directive 'server_name'");
  }
  // iterate 
  for (;";" != *it; ++it) 
  {
    if ("}" == *it)
    {
      throw Parser::ParserException("Error! missing ';' near directive 'server_name'");
    }
    // save
    this->_server_name.push_back(*it);
  }
  return ;
}

void Parser::parseRoot(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'root'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'root'");
  }

  // save 
  this->_root = *it;
  //check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'root'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'root'");
  }
  return ;
}

void Parser::parseIndex(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'index'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'index'");
  }
  // clear previous values
  this->_index.clear();
  // iterate 
  for (;";" != *it; ++it) 
  {
    // check ;
    if ("}" == *it) 
    {
      throw Parser::ParserException("Error! missing ';' near directive 'index'");
    }
    // save
    this->_index.push_back(*it);
  }
  return ;
}

void Parser::parseAutoindex(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'autoindex'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'autoindex'");
  };

  // save 
  if ("on" == *it) 
  {
    this->_autoindex = true;
  } 
  else if ("off" == *it) 
  {
    this->_autoindex = false;
  } 
  else {
    throw Parser::ParserException("Error! unknown value: '" + *it + "' in 'autoindex' directive");
  }

  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'autoindex");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'autoindex'");
  }
  return ;
}

void Parser::parseErrorPage(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'error_page'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'error_page'");
  };

  // save 
  std::vector<int> verror;
  std::map<int, std::string>::const_iterator itcs;
  //ex error_page 500 502 503 504 /50x.html; save all numbers
  while (it->find_first_not_of("0123456789") == std::string::npos) 
  {

      // if ("301" == *it || "302" == *it || "303" == *it || "307" == *it
      // || (it->length() == 3 && ('1' == (*it)[0] || '2' == (*it)[0] || '4' == (*it)[0] || '5' == (*it)[0])))  
      int number = atoi(it->c_str());
      itcs = this->_code_status.find(number);  
      if(this->_code_status.end() != itcs)
      {
        verror.push_back(atoi(it->c_str()));
       } 
      else 
      {
        throw Parser::ParserException("Error! invalid return status code in directive 'error_page'");
      }
      ++it;
  }
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'error_page'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'error_page'");
  };

  // save all pages with associated number
  for (std::vector<int>::const_iterator n = verror.begin(); n != verror.end(); ++n) 
  {
    this->_error_page[*n] = *it;
  }

  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'error_page'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'error_page'");
  }
  return ;
}

void Parser::parseLimitExcept(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'limit_except'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'limit_except'");
  }

  //clear previous values
  this->_limit_except.clear();
  // iterate methods
  for (;";" != *it; ++it) 
  {
    //check semicolon
    if ("}" == *it) 
    {
      throw Parser::ParserException("Error! missing ';' near directive 'limit_except'");
    }
    // save methods
    this->_limit_except.push_back(*it);
  }
  return ;
}

void Parser::parseClientMaxBodySize(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'client_max_body_size'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'client_max_body_size'");
  }
  //m for megabytes and k for kilobytes
  if (it->find_first_not_of("0123456789kKmM") != std::string::npos) 
  {
    throw Parser::ParserException("Error! invalid size in 'client_max_body_size' directive");
  }
  // save 
  this->_client_max_body_size = static_cast<std::string>(it->c_str());
  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'client_max_body_size'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'client_max_body_size'");
  }
  return ;
}

void Parser::parseUploadStore(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'upload_store'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'upload_store'");
  }

  // save 
  this->_upload_store = *it;

  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'upload_store'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'upload_store'");
  }
  return ;
}

void Parser::parseAlias(_type::const_iterator & it) 
{
  // check next token is not a closed brace
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'alias'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'alias'");
  }

  // save 
  this->_alias = *it;

  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'alias'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'alias'");
  }
  return ;
}

void Parser::parseReturn(_type::const_iterator & it) 
{
  // check next token is not a closed brace, return requires two values
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'return'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'return'");
  };
  std::vector<int> returnvalue;
  std::map<int, std::string>::const_iterator itcs;
  while (it->find_first_not_of("0123456789") == std::string::npos) 
    {
      // check semicolon && check status code is 1.. | 2.. | 4.. | 5.. or 3..
      // text is optional
      // if ("301" == *it || "302" == *it || "303" == *it || "307" == *it
      //     || (it->length() == 3 && ('1' == (*it)[0] || '2' == (*it)[0] || '4' == (*it)[0] || '5' == (*it)[0])))  
      int number = atoi(it->c_str());
      itcs = this->_code_status.find(number);  
      if(this->_code_status.end() != itcs)
        {
            returnvalue.push_back(atoi(it->c_str()));
        } 
      else 
      {
        throw Parser::ParserException("Error! invalid return status code in directive 'return'");
      }
      ++it;
      if ("}" == *it) 
      {
        throw Parser::ParserException("Error! missing ';' near directive 'return'");
       }
      if (";" == *it) 
      {
        for (std::vector<int>::const_iterator n = returnvalue.begin(); n != returnvalue.end(); ++n) 
         {
                this->_return[*n] = "";
         }
         return ;
       }
       //text
      if(it->find_first_not_of("0123456789") != std::string::npos)
       {
         --it;
         break ;
       }
    }
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'return'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'return'");
  }
  //save
  for (std::vector<int>::const_iterator n = returnvalue.begin(); n != returnvalue.end(); ++n) 
  {
      this->_return[*n] = *it;
  }
  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'return'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'return'");
  }
  return ;
}

void Parser::parseCgi(_type::const_iterator & it) 
{
  // check next directive is not a closed brace, cgi requires two values
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'cgi'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'cgi'");
  };

  // save 
  const std::string & extension = *it;

  // check if second value exist
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing value and ';' near directive 'cgi'");
  }
  if (";" == *it) 
  {
    throw Parser::ParserException("Error! not enough values in directive 'cgi'");
  };

  // save
  this->_cgi[extension] = *it;

  // check semicolon
  ++it;
  if ("}" == *it) 
  {
    throw Parser::ParserException("Error! missing ';' near directive 'cgi'");
  }
  if (";" != *it) 
  {
    throw Parser::ParserException("Error! too many values in directive 'cgi'");
  }
  return ;
}

Parser::ParserException::ParserException(const std::string & message) : _message(message)
{
  return ;
}

Parser::ParserException::~ParserException(void) throw () 
{
  return ;
}
const char * Parser::ParserException::what(void) const throw () 
{
  return this->_message.c_str();
}

