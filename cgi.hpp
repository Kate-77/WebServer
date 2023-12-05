#ifndef CGI_HPP
# define CGI_HPP

# include <map>
# include <string>

//class Request;
//class Response;

class Cgi {
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
    std::string       _responsebody; 

    //request cgibin
    std::string       _Bin; 
    //cgi path script
    std::string       _Path; 

    std::string       _TmpPath;
    int               _TmpFd;

    char **           _av; //av cgi
    char **           _env; //env cgi

    std::string       _body; // save cgi output into body

    //initialize env
    int initEnv( void );

  public:
    //Constructor
    Cgi( const Request & Request, const Response & Response );
    //Destructor
    ~Cgi( void );
    
    int execute( void );
    //parse headers and body from response 
    void parseHeadersAndBody( void );

};


#endif 
