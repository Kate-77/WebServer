#pragma once
#include "../request/HttpRequestParser.hpp"
#include "../conf/Parser.hpp"
#include "../cgi/cgi.hpp"
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <istream>
#include <string> 

class Parser;
class HttpRequestParser;
class CGI;

class Response
{
    public:
      int                        client_fd; //zowa
      bool                       res_initialized; //zowa
      bool                       client_done; //zowa
	    size_t					sent;
      std::string                _file;
      std::string                _file_path; // path of file
	    std::string				 _post_file_name;
      std::fstream               _file_fd; // file descriptor
      std::string                _cgi_bin;
      std::string                _response; // response to send to client
      std::string                _body; // body of response
      std::string                _head; // head of response
      int                        _status_code; // status code
      std::string                _locationPath; // location of resource
      Parser                     *_location; // server
      std::string                _contentType; // content type of resource
      size_t                     _contentLength; // content length of resource
      ssize_t                    _errorContentLength;
      std::map<int, std::string> _status_codes; // map of status code and status message

      Response();
      ~Response();
      Response(Response const & src);
      Response & operator=(Response const & rhs);

      //void        setResponse(HttpRequestParser & request, Parser & server); //set response to send to client
      //void        setBody(HttpRequestParser & request, Parser & server); // set body of response
      //void        setStatus(HttpRequestParser & request, Parser & server); // set status code
      //void        setLocation(HttpRequestParser & request, Parser & server); // set location of resource
      //void        setContentType(HttpRequestParser & request, Parser & server); // set content type of resource
      //void        setContentLength(HttpRequestParser & request, Parser & server); // set content length of resource
      
      void        handleResponse(HttpRequestParser & request, Parser &server);
      int         findLocation(Parser &server, const std::string& path);
      void        callErrorPage(Parser& server, int code);
      void        generateErrorPage(int code);
      std::string statusMessage(int code);
      void        handleDeleteRequest(HttpRequestParser &request, Parser &server);
      void        handleGetRequest(HttpRequestParser &request, Parser &server);
      void        handlePostRequest(HttpRequestParser &request, Parser &server);
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
      void        handleDirFile(HttpRequestParser &request, Parser &server, const std::string &path);

      //std::string getResponse(); // get response to send to client
      //std::string getBody(); // get body of response
      //std::string getStatus(); // get status code
      //std::string getLocation(); // get location of resource
      //std::string getContentType(); // get content type of resource
      std::string getContentLength(); // get content length of resource   
};

std::string getFileType(const std::string& filePath);
std::string getExtension(const std::string &path);
std::string getExtensionFromType(const std::string &contentType);
bool        checkDirectory(std::string path);
bool        checkDirectoryEmpty(std::string path);
std::string printNumber(int value);
std::string repetetiveSlash(std::string file);
bool endSlash(std::string file);
std::string createPath(const std::string &path);
std::string generateName();