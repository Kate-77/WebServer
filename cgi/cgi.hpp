#ifndef CGI_HPP
# define CGI_HPP

# include <map>
# include <string>
# include <sstream>
# include <iostream>
# include <unistd.h> 
# include <sys/wait.h> 
# include <fcntl.h> 
# include <unistd.h> 
# include <sys/stat.h> 
# include <dirent.h> 
# include <algorithm>
# include <cstring> 

//class Request;
//class Response;

const size_t        Default_buffer = 8192;
const std::string   end_of_file = "\r\n";
const std::string random_tmp_file = "/tmp/a";

class CGI {
  private:
    //fd 0
    static const int fRead = 0;
    //fd 1
    static const int fWrite = 1;
    // get : Body  Header CGI Methods Request query Host Port Version ->request 
    // get : header body extension path -> response

    std::string       _requestBody; 
    std::string       _requestFilePath; 
    std::map<std::string, std::string>  _responseheaders;//content-length/type: description, location: description
    // std::string       _responsebody; inutile 

    //request cgibin
    std::string       _Bin; 
    //cgi path script
    std::string       _Path; 

    std::string       _TmpPath;
    int               _TmpFd;

    char **           _av; //av cgi
    char **           _env; //env cgi
    //env has parsed request and meta-variables

    std::string       _body; // save cgi output into body

    //initialize env
    int initEnv( void );

  public:
    //Constructor
    // CGI( const Request & Request, const Response & Response );
    CGI(void);
    //Destructor
    ~CGI( void );
    
    int execute( void );
    //parse headers and body from response
    // void parseHeadersAndBody(std::map<std::string, std::string> & headers, std::string & body);
    void parseHeadersAndBody( void);

};


#endif 
