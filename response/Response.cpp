# include "Response.hpp"

Response::Response()
{
    _response = ""; // response to send to client
    _body = ""; // body of response
    _status_code = 0; // status code
    _contentType = ""; // content type of resource
    _contentLength = 0; // content length of resource
    _head = ""; // head of response
    _location = NULL; // location of resource
	client_fd = -1;
	res_initialized = false;
	client_done = false;
    sent = 0;
}

Response::~Response()
{
    return ;
}

Response::Response(Response const & src)
{
    *this = src;
    return ;
}

Response & Response::operator=(Response const & rhs)
{
    if (this != &rhs)
    {
        this->_response = rhs._response;
        this->_body = rhs._body;
        this->_head = rhs._head;
        this->_status_code = rhs._status_code;
        this->_location = rhs._location;
        this->_contentType = rhs._contentType;
        this->_contentLength = rhs._contentLength;
        this->_file_path = rhs._file_path;
        this->_file_fd.open(rhs._file_path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        this->_file_fd << rhs._file_fd.rdbuf();	
    }
    return (*this);
}

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// Starting here
void    Response::handleResponse(HttpRequestParser & request, Parser &server)
{
    
    if (request.getStatusCode() != -1) 
    {
        this->_status_code = request.getStatusCode();
        callErrorPage(server, _status_code);
        return;
    }
    int foundLocation = findLocation(server, request.getPath());
    if (foundLocation == -1)
    {
        callErrorPage(server, 404);
        return;
    }
    printf("location: %s\n", this->_locationPath.c_str());
    printf("start3\n");
    if (std::find(this->_location->getLimit_except().begin(), this->_location->getLimit_except().end(), request.getMethod()) == this->_location->getLimit_except().end())
    {
        callErrorPage(server, 405);
        return;
    }
    else {
        if (request.getMethod() == "GET")
            handleGetRequest(request, server);
        else if (request.getMethod() == "POST")
            handlePostRequest(request, server);
        else if (request.getMethod() == "DELETE")
            handleDeleteRequest(request, server);
        else
            callErrorPage(server, 405);
    }
    return ;
}

// Errors
void Response::callErrorPage(Parser& server, int code) 
{
    std::string path;
    const std::map<int, std::string> &_error_pages = server.getError_page();

    if (code == 301) 
    {
        // Handle redirect separately
        this->_head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nLocation: " + this->_locationPath + "/" + "\r\n\r\n";
        return;
    }
    printf("callError\n");
    if (_error_pages.find(code) != _error_pages.end()) {
        path = _error_pages.find(code)->second;
        this->_file_path = path;
        this->_file_fd.open(path, std::ios::in | std::ios::binary | std::ios::ate);

        if (this->_file_fd.is_open()) {
            this->_errorContentLength = this->_file_fd.tellg();
            this->_file_fd.seekg(0, std::ios::beg);

            this->_head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(this->_errorContentLength) + "\r\n\r\n";
        } 
        else
            generateErrorPage(code);
    }
    else
        generateErrorPage(code);
}

// Delete Method
void    Response::handleDeleteRequest(HttpRequestParser &request, Parser &server) 
{
    std::string path;
    path = request.getPath();

    // check file existence
    if (access(path.c_str(), F_OK) == -1)
        callErrorPage(server, 404);
    // if it exists, we check if it's writable to be able to delete it
    else if (access(path.c_str(), W_OK) == -1)
        callErrorPage(server, 403);
    else if (checkDirectory(path))
    {
        // if it's a dir, we check if it's empty
        if (checkDirectoryEmpty(path)) {
            if (rmdir(path.c_str()))
                callErrorPage(server, 403);
            else
                callErrorPage(server, 204);
        } 
        else
            callErrorPage(server, 403);
    }
    else if (std::remove(path.c_str()))
        callErrorPage(server, 403);
    else
        callErrorPage(server, 204);
}

// Get Method
void Response::handleGetRequest(HttpRequestParser &request, Parser &server)
{
    std::string file = constructFilePath(request.getPath());
    if (checkDirectory(file))
    {
        printf("watti \n");
        handleDirectoryGet(file, request, server);}
    else
        handleFileGet(file, request, server);
}

// Post Method
void Response::handlePostRequest(HttpRequestParser &request, Parser &server)
{
    std::string file = constructFilePath(request.getPath());
    std::string extension = getExtension(file);

    if (_location->getUpload_store() == false)
        handleFilePost(request, server, file);
    else if (checkDirectory(request.getPath()))
        handleDirectoryPost(request, server, file);
    else
        handleDirFile(request, server, file);
}