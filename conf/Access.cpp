#include "Access.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

void Access::start(void)
{
    try
    {
      this->_Lexer = new Lexer(); //ici we create a lexer because the Lexer divide each server
      this->_Lexer->start(NULL); // ici  we start Parsing each server
      this->_Servers = _Lexer->getparserv(); // ici we get all servers
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return ;
    }
    //ici we iterate server by server
    for ( std::vector<Parser *>::const_iterator it = this->_Servers->begin(); it != this->_Servers->end(); ++it )
    {
      static int n;
      std::cout << std::endl << "server number " << n++ << "{" <<std::endl;
      std::vector<address_port *> &       listens =  (*it)->getListen();
      if(listens.empty() == false)
      {
          for (std::vector<address_port *>::const_iterator its = listens.begin(); its != listens.end(); ++its) 
          {
            std::cout <<  "address: " << (*its)->_address << " port: " << (*its)->_port << std::endl;
          }
      }
      std::vector<std::string> &         server_names =   (*it)->getServerName();
      if(server_names.empty() == false)
      {
        for (std::vector<std::string>::const_iterator its = server_names.begin(); its != server_names.end(); ++its) 
        {
             std::cout << "Server name s:   " << *its << std::endl;
        }
      }

      std::string &                        root =  (*it)->getRoot();
      if(root.empty() == false)
        std::cout << "root s :" << root << std::endl;

      std::vector<std::string>         &   indexes =  (*it)->getIndex(); 
      if(indexes.empty() == false)
      {
        for (std::vector<std::string>::const_iterator its = indexes.begin(); its != indexes.end(); ++its) 
        {
          std::cout << " index s:   " << *its << std::endl;
        }
      }

      bool        &                      autoindex =    (*it)->getAutoindex(); 
      if(autoindex)
        std::cout << "autoindex: " << std::boolalpha << autoindex << std::endl;

      std::map<int, std::string>        error_pages =    (*it)->getError_page(); 
      if(error_pages.empty() == false)
      {
            for (std::map<int, std::string>::const_iterator its = error_pages.begin(); its != error_pages.end(); ++its) 
            {
              std::cout << "error_page s :" << its->first << " " << its->second << std::endl;
            }
      }

      std::vector<std::string>            Limit_except =   (*it)->getLimit_except(); 
      if(Limit_except.empty() == false)
      {
          for (std::vector<std::string>::const_iterator its = Limit_except.begin(); its != Limit_except.end(); ++its) 
          {
            std::cout << "limit_except s:" << *its << std::endl;
          }
      }

      std::string &                       max_body =   (*it)->getClient_max_body_size(); 
      if(max_body.empty() == false)    
        std::cout << "client_max_body_size: " << max_body << std::endl;
      
      std::string &                       upload_store =   (*it)->getUpload_store(); 
      if(upload_store.empty() == false)
        std::cout << "upload_store:         " << upload_store << std::endl;
  
      std::map<std::string, std::string>   ccgi = (*it)->getCgi();
      if(ccgi.empty() == false)
      {
            for (std::map<std::string, std::string>::const_iterator its = ccgi.begin(); its != ccgi.end(); ++its) 
            {
              std::cout << "cgi s:" << its->first << " -> " << its->second << std::endl;
            }

      }
      std::map<int, std::string>          returnnnn =  (*it)->getReturn();
      if(returnnnn.empty() == false) 
      {
        for (std::map<int, std::string>::const_iterator its = returnnnn.begin(); its != returnnnn.end(); ++its) 
        {
          std::cout << "return s :" << its->first << " " << its->second << std::endl;
        }
      }

      std::map<std::string, Parser *> & locations = (*it)->getLocation(); //ici we get Location  for ech server
      for (std::map<std::string, Parser *>::const_iterator b = locations.begin();b != locations.end(); ++b)
      {
        std::cout << std::endl << "Location     " << b->first <<  "{" << std::endl;
        // b->second->printparser("----Location Block----"); //ici you can print all dir    
      
      std::string &                       lalias =  b->second->getAlias(); 
      if(lalias.empty() == false)
         std::cout << "   alias l: " << lalias << std::endl;

      std::string &                        lroot =  b->second->getRoot();
      if(lroot.empty() == false)
        std::cout << "    root l :" << lroot << std::endl;

      std::vector<std::string>         &   lindexes =  b->second->getIndex(); 
      if(lindexes.empty() == false)
      {
        for (std::vector<std::string>::const_iterator its = lindexes.begin(); its != lindexes.end(); ++its) 
        {
          std::cout << "  index l:   " << *its << std::endl;
        }
      }

      std::map<int, std::string>          lreturnnnn =  b->second->getReturn();
      if(lreturnnnn.empty()) 
      {
        for (std::map<int, std::string>::const_iterator its = lreturnnnn.begin(); its != lreturnnnn.end(); ++its) 
        {
          std::cout << "    return l :" << its->first << " " << its->second << std::endl;
        }
      }
      bool        &                      lautoindex =    b->second->getAutoindex();
      if(autoindex)
        std::cout << "    autoindex l : " << std::boolalpha << lautoindex << std::endl;
      
      std::map<int, std::string>        lerror_pages =    b->second->getError_page(); 
      if(lerror_pages.empty() == false)
      {
            for (std::map<int, std::string>::const_iterator its = lerror_pages.begin(); its != lerror_pages.end(); ++its) 
            {
              std::cout << "    error_page l :" << its->first << " " << its->second << std::endl;
            }
      }
      std::vector<std::string>            lLimit_except =   b->second->getLimit_except(); 
      if(lLimit_except.empty() == false)
      {
          for (std::vector<std::string>::const_iterator its = lLimit_except.begin(); its != lLimit_except.end(); ++its) 
          {
            std::cout << "    limit_except l:" << *its << std::endl;
          }
      }
      std::string &                       lmax_body =   b->second->getClient_max_body_size(); 
      if(lmax_body.empty() == false)    
        std::cout << "    client_max_body_size l: " << lmax_body << std::endl;
      std::string &                       lupload_store =   b->second->getUpload_store(); 
      if(lupload_store.empty() == false)
        std::cout << "    upload_store l:         " << lupload_store << std::endl;

      std::map<std::string, std::string>   lccgi = b->second->getCgi();
      if(lccgi.empty() == false)
      {
            for (std::map<std::string, std::string>::const_iterator its =lccgi.begin(); its !=lccgi.end(); ++its) 
            {
              std::cout << "    cgi l:" << its->first << " -> " << its->second << std::endl;
            }

      }
      std::cout << "    }" << std::endl;
        // b->second->getx() // ici you can acces to all directives here
      } 
      std::cout << "}" << std::endl;
    }
}