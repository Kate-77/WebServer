# include "Response.hpp"

Response::Response()
{
    _response = ""; // response to send to client
    _body = ""; // body of response
    _status_code = 0; // status code
    _content_type = ""; // content type of resource
    _content_length = 0; // content length of resource
    _head = ""; // head of response
    _location = NULL; // location of resource

	client_fd = -1;
	res_initialized = false;
	client_done = false;
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
        this->_content_type = rhs._content_type;
        this->_content_length = rhs._content_length;
        this->_file_path = rhs._file_path;
        this->_file_fd = rhs._file_fd;
		
    }
    return (*this);
}

std::ostream & operator<<(std::ostream & o, Response const & rhs)
{
    o << rhs.getResponse();
    return (o);
}

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// starting here
void    Response::handleResponse(HttpRequestParser & request, Parser &server)
{
    this->_location = findLocation(server.getLocation(), request.getUrl());
    
    if (this->_location == server.getLocation().end()) 
    {
        callErrorPage(server, 404);
        return;
    }

    if (request.status_code != 0) {
        this->_status_code = request.status_code;
        callErrorPage(server, _status_code);
        return;
    }

    if (this->_location->getAllowedMethods().find(request.getMethod()) == this->_location->getAllowedMethods().end()) {
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
    }
    return ;
}

// Errors
void Response::callErrorPage(Parser server, int code) 
{
    std::string path;
    const std::map<int, std::string> &_error_pages = server.getError_page();

    if (code == 301) {
        // Handle redirect separately
        this->head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nLocation: " + this->location + "/" + "\r\n\r\n";
        return;
    }

    if (_error_pages.find(code) != _error_pages.end()) {
        path = _error_pages.find(code)->second;
        this->_file_path = path;
        this->_file_fd.open(path, std::ios::in | std::ios::binary | std::ios::ate);

        if (this->_file_fd.is_open()) {
            this->_content_length = this->_file_fd.tellg();
            this->_file_fd.seekg(0, std::ios::beg);

            this->head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(this->_content_length) + "\r\n\r\n";
        } else {
            generateErrorPage(code);
        }
    } else {
        generateErrorPage(code);
    }
}

// Delete Method
void Response::handleDeleteRequest(HttpRequestParser &request, Server &server) 
{
    std::string path;
    path = request.getUrl();

    // check file existence
    if (access(path.c_str(), F_OK) == -1)
        callErrorPage(server, 404);
    // if it exists, we check if it's writable to be able to delete it
    else if (access(path.c_str(), W_OK) == -1)
        callErrorPage(server, 403);
    else if (checkDirectory(path)) {
        // if it's a dir, we check if it's empty
        if (checkDirectoryEmpty(path)) {
            if (rmdir(path.c_str()))
                callErrorPage(server, 403);
            else
                callErrorPage(server, 204);
        } else {
            callErrorPage(server, 403);
        }
    } else if (std::remove(path.c_str()))
        callErrorPage(server, 403);
    else
        callErrorPage(server, 204);
}

void Response::handleGet(HttpRequestParser &request, Parser &server) 
{
    std::string file = constructFilePath(request.getUrl());

    if (checkDirectory(file))
        handleDirectoryGet(file, request, server);
    else
        handleFileGet(file, request, server);
}

void Response::handlePost(HttpRequestParser &request, Parser &server)
{
    std::string file = constructFilePath(request.getUrl());
    std::string extension = getExtension(file);

    if (!_location->getUpload().empty())
        handleFilePost(request, server, file);
    else if (checkDirectory(request.getUrl()))
        handleDirectoryPost(request, server, file);
    else
        handleFileUpload(request, server, file);
}
