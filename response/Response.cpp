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
    if (request.getMethod() == "POST")
    {
        unsigned int bytes = atoi(request.getHeadersMap().find("Content-Length")->second.c_str());
        if (bytes > server.getbodysizebytes())
        {
            generateErrorPage(413);
            return ;
        }
    }

	int foundLocation = findLocation(server, request.getPath());
    if (foundLocation == -1)
    {
        printf("no location found\n");
        // callErrorPage(server, 404);
        generateErrorPage(404);
        return;
    }
	
	std::cout << " request URL " << request.getPath() << std::endl; // HNA MOCHKIL KBIR REDIRECT
    // request parsing throwing an error
    if (request.getStatusCode() != -1)
    {
	    std::cout << "status code coming from request: " << request.getStatusCode() << std::endl;
        this->_status_code = request.getStatusCode();
        callErrorPage(server, _status_code);
        return;
    }
    // non allowed method
    std::vector<std::string>::iterator it1 = this->_location->getLimit_except().begin();
    std::vector<std::string>::iterator it2 = this->_location->getLimit_except().end();
    // while (it != this->_location->getLimit_except().end())
    // {
    //     if (*it == request.getMethod())
    //     {
        std::cout << *it1 <<  request.getMethod() << std::endl;
    if (!this->_location->getLimit_except().empty())
    {
        //std::cout << std::find(this->_location->getLimit_except().begin()
    //if (std::find(this->_location->getLimit_except().begin(), this->_location->getLimit_except().end(), request.getMethod()) == this->_location->getLimit_except().end())
    if (std::find(it1, it2, request.getMethod()) == it2)
    {
        callErrorPage(server, 405);
        return;
    }
    else {
        if (_location->getReturn().size() != 0)
        {
            printf("redirection path: %s\n", _location->getReturn().c_str());
            this->_head = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + _location->getReturn() + "\r\n\r\n";
        }
        else if (request.getMethod() == "GET")
            handleGetRequest(request, server);
        else if (request.getMethod() == "POST")
            handlePostRequest(request, server);
        else if (request.getMethod() == "DELETE")
            handleDeleteRequest(request, server);
        else
            callErrorPage(server, 405);
    }}
    else
            callErrorPage(server, 405);
    return ;
}

// Errors
void Response::callErrorPage(Parser& server, int code) 
{
    std::string path;

	printf("call error page start, code: %d\n", code);
    std::map<int, std::string> _error_pages = _location->getError_page();
    if (_error_pages.empty())
        _error_pages = server.getError_page();

    if (code == 301)
    {
        this->_head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nLocation: " + _location->getReturn().c_str() + "/" + "\r\n\r\n";
        return;
    }

    if (!_error_pages.empty())
    {
        if (_error_pages.find(code) != _error_pages.end()) { // TO DEBUG
            this->_file_path = _error_pages.find(code)->second ;
            this->_file_fd.open(_file_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            if (this->_file_fd.is_open()) {
                this->_errorContentLength = this->_file_fd.tellg();
                this->_contentLength = this->_file_fd.tellg();
                this->_file_fd.seekg(0, std::ios::beg);
                this->_head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(this->_contentLength) + "\r\n\r\n";
                _file_fd.close();
            } 
            else
                generateErrorPage(code);
        }
        else
            generateErrorPage(code);
    }
    else
        generateErrorPage(code);
}

// Delete Method
void    Response::handleDeleteRequest(HttpRequestParser &request, Parser &server) // TO TEST
{
    printf("#################  DELETE  ###############\n");
	// construct path
	this->_file_path = constructFilePath(request.getPath());
    printf("file DELETE : %s\n", _file_path.c_str());
    // check file existence 
    if (access(_file_path.c_str(), F_OK) == -1)
        callErrorPage(server, 404);
    // if it exists, we check if it's writable to be able to delete it
    else if (access(_file_path.c_str(), W_OK) == -1)
        callErrorPage(server, 403);
    else if (checkDirectory(_file_path))
    {
        // if it's a dir, we check if it's empty
        if (checkDirectoryEmpty(_file_path)) {
            if (rmdir(_file_path.c_str()))
                callErrorPage(server, 403);
            else
                callErrorPage(server, 204);
        } 
        else
            callErrorPage(server, 403);
    }
    else if (std::remove(_file_path.c_str()))
        callErrorPage(server, 403);
    else
        callErrorPage(server, 204);
}

// Get Method
void Response::handleGetRequest(HttpRequestParser &request, Parser &server)
{
    printf("#################  GET  ###############\n");

    printf("request file coming: %s\n", request.getPath().c_str());
    std::string file = constructFilePath(request.getPath());
    printf("file GET after: %s\n", file.c_str());

    if (checkDirectory(file))
        handleDirectoryGet(file, request, server);
    else
        handleFileGet(file, request, server);
}

// Post Method
void Response::handlePostRequest(HttpRequestParser &request, Parser &server)
{
    printf("#################  POST  ###############\n");
    std::string file = constructFilePath(request.getPath());

    printf("file POST after construction: %s\n", file.c_str());

	// CHECK when POST doesnt have a body response
    if (_location->getUpload_store() == true)
        handleFilePost(request, server, file);
    else if (checkDirectory(request.getPath())) // HOW TO TEST THIS?
	{
		//printf("ma allo \n");
        handleDirectoryPost(request, server, file);
	}
		
    else //CG left
	{
		printf(" ma3ert  \n");
        handleDirFile(request, server, file);
	}
}