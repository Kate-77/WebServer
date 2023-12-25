# include "Response.hpp"

Response::Response()
{
    return ;
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

void    Response::setResponse(Request & request, WebServ & server)
{
    this->setBody(request, server);
    this->setStatus(request, server);
    this->setLocation(request, server);
    this->setContentType(request, server);
    this->setContentLength(request, server);
    this->setServer(request, server);
    this->setError(request, server);
    this->setErrorStatus(request, server);
    this->_response = this->_status + this->_location + this->_content_type + this->_content_length + this->_server + this->_error + this->_error_status + "\r\n" + this->_body;
    return ;
}

const  std::map<int, std::string> _status_code = {
    {100, "Continue"},
    {101, "Switching Protocols"},
    {102, "Processing"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {207, "Multi-Status"},
    {208, "Already Reported"},
    {226, "IM Used"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {306, "Switch Proxy"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Timeout"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Payload Too Large"},
    {414, "URI Too Long"},
    {415, "Unsupported Media Type"},
    {416, "Range Not Satisfiable"},
    {417, "Expectation Failed"},
    {418, "I'm a teapot"},
    {421, "Misdirected Request"},
    {422, "Unprocessable Entity"},
    {423, "Locked"},
    {424, "Failed Dependency"},
    {425, "Too Early"},
    {426, "Upgrade Required"},
    {428, "Precondition Required"},
    {429, "Too Many Requests"},
    {431, "Request Header Fields Too Large"},
    {451, "Unavailable For Legal Reasons"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
    {505, "HTTP Version Not Supported"},
    {506, "Variant Also Negotiates"},
    {507, "Insufficient Storage"},
    {508, "Loop Detected"},
    {510, "Not Extended"},
    {511, "Network Authentication Required"}
};

void    Response::setStatus(Request & request, WebServ & server)
{
    std::string     status;
    std::string     status_code;
    std::string     status_message;
    std::string     version;

    version = request.getVersion();
    status_code = request.getStatus();
    status_message = _status_code[atoi(status_code.c_str())];
    status = version + " " + status_code + " " + status_message + "\r\n";
    this->_status = status;
    return ;
}

void    Response::setLocation(Request & request, WebServ & server)
{
    std::string     location;
    std::string     uri;

    uri = request.getUri();
    location = "Location: " + uri + "\r\n";
    this->_location = location;
    return ;
}

void    Response::setBody(Request & request, WebServ & server)
{
    std::string     body;
    std::string     line;
    std::ifstream   file;
    std::string     path; 

    path = server.getRoot() + request.getUri(); // getRoot : is to get the root of the server and getUri : is to get the uri of the request
    if (request.getMethod() == "GET")
    {
        file.open(path.c_str());
        if (file.is_open())
        {
            while (getline(file, line))
                body += line + "\n";
            file.close();
        }
        else
            body = "";
    }
    else
        body = "";
    this->_body = body;
    return ;
}

void    Response::setContentType(Request & request, WebServ & server)
{
    std::string     content_type;
    std::string     extension;
    std::string     path;
    std::string     line;
    std::ifstream   file;

    path = server.getRoot() + request.getUri();
    file.open(path.c_str());
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Content-Type:") != std::string::npos)
            {
                content_type = line.substr(line.find(":") + 2);
                break ;
            }
        }
        file.close();
    }
    else
        content_type = "text/html";
    this->_content_type = "Content-Type: " + content_type + "\r\n";
    return ;
}

void    Response::setContentLength(Request & request, WebServ & server)
{
    std::string     content_length;
    std::string     path;
    std::string     line;
    std::ifstream   file;
    int             length;

    path = server.getRoot() + request.getUri();
    file.open(path.c_str());
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Content-Length:") != std::string::npos)
            {
                content_length = line.substr(line.find(":") + 2);
                break ;
            }
        }
        file.close();
    }
    else
    {
        file.open(server.getErrorPage().c_str());
        if (file.is_open())
        {
            while (getline(file, line))
                length += line.length();
            file.close();
        }
        else
            length = 0;
        content_length = ft_itoa(length);
    }
    this->_content_length = "Content-Length: " + content_length + "\r\n";
    return ;
}

void    Response::setServer(Request & request, WebServ & server)
{
    std::string     server_name;

    server_name = server.getServerName();
    this->_server = "Server: " + server_name + "\r\n";
    return ;
}

void    Response::setError(Request & request, WebServ & server)
{
    std::string     error;
    std::string     path;
    std::string     line;
    std::ifstream   file;

    path = server.getErrorPage();
    file.open(path.c_str());
    if (file.is_open())
    {
        while (getline(file, line))
            error += line + "\n";
        file.close();
    }
    else
        error = "";
    this->_error = error;
    return ;
}

void    Response::setErrorStatus(Request & request, WebServ & server)
{
    std::string     error_status;
    std::string     status_code;
    std::string     status_message;
    std::string     version;

    version = request.getVersion();
    status_code = request.getStatus();
    status_message = _status_code[atoi(status_code.c_str())];
    error_status = version + " " + status_code + " " + status_message + "\r\n";
    this->_error_status = error_status;
    return ;
}

std::string     Response::getResponse()
{
    return (this->_response);
}

Response & Response::operator=(Response const & rhs)
{
    if (this != &rhs)
    {
        this->_response = rhs._response;
        this->_status = rhs._status;
        this->_location = rhs._location;
        this->_content_type = rhs._content_type;
        this->_content_length = rhs._content_length;
        this->_server = rhs._server;
        this->_error = rhs._error;
        this->_error_status = rhs._error_status;
        this->_body = rhs._body;
    }
    return (*this);
}

std::ostream & operator<<(std::ostream & o, Response const & rhs)
{
    o << rhs.getResponse();
    return (o);
}