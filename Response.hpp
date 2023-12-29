#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp" // TBD
#include "WebServ.hpp" // TBD

class Response
{
    public:
        Response();
        ~Response();
        Response(Response const & src);
        Response & operator=(Response const & rhs);

        void    setResponse(Request & request, WebServ & server); //set response to send to client
        void    setBody(Request & request, WebServ & server); // set body of response
        void    setStatus(Request & request, WebServ & server); // set status code
        void    setLocation(Request & request, WebServ & server); // set location of resource
        void    setContentType(Request & request, WebServ & server); // set content type of resource
        void    setContentLength(Request & request, WebServ & server); // set content length of resource
        void    setServer(Request & request, WebServ & server); // set server name
        void    setError(Request & request, WebServ & server); // set error message
        void    setErrorStatus(Request & request, WebServ & server); // set error status code

        std::string getResponse(); // get response to send to client
        std::string getBody(); // get body of response
        std::string getStatus(); // get status code
        std::string getLocation(); // get location of resource
        std::string getContentType(); // get content type of resource
        std::string getContentLength(); // get content length of resource
        std::string getServer(); // get server name
        std::string getError(); // get error message 
        std::string getErrorStatus(); // get error status code
    
    private:
        std::string _response; // response to send to client
        std::string _body; // body of response
        std::string _status; // status code
        std::string _location; // location of resource
        std::string _content_type; // content type of resource
        std::string _content_length; // content length of resource
        std::string _server; // server name
        std::string _error; // error message
        std::string _error_page;
        std::string _error_status; // error status code
        std::map<int, std::string> _status_code; // map of status code and status message
};

#endif