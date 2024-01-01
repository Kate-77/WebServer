#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../request/HttpRequestParser.hpp"
#include "../conf/Parser.hpp" // TBD

class Parser;
class HttpRequestParser;

class Response
{
    public:
        Response();
        ~Response();
        Response(Response const & src);
        Response & operator=(Response const & rhs);

        void        setResponse(HttpRequestParser & request, Parser & server); //set response to send to client
        void        setBody(HttpRequestParser & request, Parser & server); // set body of response
        void        setStatus(HttpRequestParser & request, Parser & server); // set status code
        void        setLocation(HttpRequestParser & request, Parser & server); // set location of resource
        void        setContentType(HttpRequestParser & request, Parser & server); // set content type of resource
        void        setContentLength(HttpRequestParser & request, Parser & server); // set content length of resource
        
        void        handleResponse(HttpRequestParser & request, Parser &server);
        Parser      *findLocation(const std::map<std::string, Parser *>& locations, const std::string& path);
        void        callErrorPage(Parser server, int code);
        void        generateErrorPage(int code);
        std::string statusMessage(int code);
        void        handleDeleteRequest(HttpRequestParser &request, Parser &server);
        void        handleGet(HttpRequestParser &request, Parser &server);
        void        handlePost(HttpRequestParser &request, Parser &server);
        //helpers:
        void        handleDirectoryGet(const std::string &directoryPath, HttpRequestParser &request, Parser &server);
        void        handleFileGet(const std::string &filePath, HttpRequestParser &request, Parser &server);
        void        handleCgiOrFileGet(HttpRequestParser &request, const std::string &path, Parser &server);
        void        handleCgi(HttpRequestParser &request, const std::string &cgiPath, Parser &server);
        void        serveFile(const std::string &filePath, Parser &server);
        void        renderFile(Parser &server, const std::string &file);
        std::string constructFilePath(const std::string &path);
        void        listDirectory(std::string file, HttpRequestParser &request, Parser &server);
        void        handleFilePost(HttpRequestParser &request, Parser &server, const std::string &file);
        void        handleDirectoryPost(HttpRequestParser &request, Parser &server, const std::string &file);
        void        handleDirectoryWithIndex(HttpRequestParser &request, Parser &server, const std::string &file);
        void        handleFileUpload(HttpRequestParser &request, Parser &server, const std::string &file);

        std::string getResponse(); // get response to send to client
        std::string getBody(); // get body of response
        std::string getStatus(); // get status code
        std::string getLocation(); // get location of resource
        std::string getContentType(); // get content type of resource
        std::string getContentLength(); // get content length of resource
    
    private:
        std::string     _response; // response to send to client
        std::string     _body; // body of response
        std::string     _head; // head of response
        int             _status_code; // status code
        std::string     _locationPath; // location of resource
        Parser          *_location; // server
        std::string     _content_type; // content type of resource
        int             _content_length; // content length of resource
        std::string     _file_path;
        std::fstream    _file_fd;
        std::map<std::string, Parser *>& locations; // map of locations
        std::map<int, std::string> _status_code; // map of status code and status message
        
};

std::string getFileType(const std::string& filePath);
std::string getExtension(const std::string &path);
std::string getExtensionFromType(const std::string &contentType);
bool        checkDirectory(std::string path);
bool        checkDirectoryEmpty(std::string path);
std::string printNumber(int value);
std::string repetetiveSlash(std::string file);
static bool endSlash(std::string file);
std::string createPath(const std::string &path);
std::string generateName();

#endif