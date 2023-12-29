# include "Response.hpp"
# include "Request.hpp"

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

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

void    Response::setResponse(Request & request, Server &server)
{
    if (request.status_code != 0)
    {
        callErrorPage(server, request.status_code);
        return;
    }
    if (request.getMethod() == "GET")
        handleGetRequest(request, server);
    else if (request.getMethod() == "POST")
        handlePostRequest(request, server);
    else if (request.getMethod() == "DELETE")
        handleDeleteRequest(request, server);
    else
        callErrorPage(server, 405);
    this->setBody(request.getBody());
    this->setStatus(request.getStatusCode());
    this->setLocation(request.getURI());
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

//generate files types
void    Response::generateFilesTypes()
{
    this->_files_types[".aac"] = "audio/aac";
    this->_files_types[".css"] = "text/css";
    this->_files_types[".csv"] = "text/csv";
    this->_files_types[".doc"] = "application/msword";
    this->_files_types[".epub"] = "application/epub+zip";
    this->_files_types[".gif"] = "image/gif";
    this->_files_types[".htm"] = "text/html";
    this->_files_types[".html"] = "text/html";
    this->_files_types[".ics"] = "text/calendar";
    this->_files_types[".jar"] = "application/java-archive";
    this->_files_types[".jpeg"] = "image/jpeg";
    this->_files_types[".jpg"] = "image/jpeg";
    this->_files_types[".js"] = "application/javascript";
    this->_files_types[".json"] = "application/json";
    this->_files_types[".mpeg"] = "video/mpeg";
    this->_files_types[".mpkg"] = "application/vnd.apple.installer+xml";
    this->_files_types[".odp"] = "application/vnd.oasis.opendocument.presentation";
    this->_files_types[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    this->_files_types[".odt"] = "application/vnd.oasis.opendocument.text";
    this->_files_types[".oga"] = "audio/ogg";
    this->_files_types[".ogv"] = "video/ogg";
    this->_files_types[".ogx"] = "application/ogg";
    this->_files_types[".otf"] = "font/otf";
    this->_files_types[".png"] = "image/png";
    this->_files_types[".pdf"] = "application/pdf";
    this->_files_types[".ppt"] = "application/vnd.ms-powerpoint";
    this->_files_types[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    this->_files_types[".rar"] = "application/x-rar-compressed";
    this->_files_types[".rtf"] = "application/rtf";
    this->_files_types[".sh"] = "application/x-sh";
    this->_files_types[".svg"] = "image/svg+xml";
    this->_files_types[".swf"] = "application/x-shockwave-flash";
    this->_files_types[".tar"] = "application/x-tar";
    this->_files_types[".tif"] = "image/tiff";
    this->_files_types[".tiff"] = "image/tiff";
    this->_files_types[".ts"] = "application/typescript";
    this->_files_types[".ttf"] = "font/ttf";
    this->_files_types[".vsd"] = "application/vnd.visio";
    this->_files_types[".wav"] = "audio/x-wav";
    this->_files_types[".weba"] = "audio/webm";
    this->_files_types[".webm"] = "video/webm";
    this->_files_types[".webp"] = "image/webp";
    this->_files_types[".xhtml"] = "application/xhtml+xml";
    this->_files_types[".xls"] = "application/vnd.ms-excel";
    this->_files_types[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    this->_files_types[".xml"] = "application/xml";
    this->_files_types[".xul"] = "application/vnd.mozilla.xul+xml";
    this->_files_types[".zip"] = "application/zip";
    this->_files_types[".7z"] = "application/x-7z-compressed";
    return ;
}

void    Response::setContentType(std::string extension)
{
    this->_content_type = "Content-Type: " + this->_files_types[extension] + "\r\n";
    return ;
}

void    Response::setContentLength(std::string length)
{
    this->_content_length = "Content-Length: " + length + "\r\n";
    return ;
}

std::string Response::statusMessage(int code) {
    auto it = _status_code.find(code);
    if (it != _status_code.end()) {
        return it->second;
    } else {
        return "Unknown Status";
    }
}

std::string Response::printNumber(int value) {
    if (value == 0) {
        return "0";
    }

    std::string result;
    bool is_negative = false;

    if (value < 0) {
        is_negative = true;
        value = -value;
    }

    while (value > 0) {
        char digit = '0' + (value % 10);
        result = digit + result;
        value /= 10;
    }

    if (is_negative) {
        result = '-' + result;
    }

    return result;
}

void Response::generateErrorPage(int code) {
    std::string errorMessage = statusMessage(code);
    std::string errorBody = "<!DOCTYPE html>\n";
    errorBody += "<html>\n";
    errorBody += "<head>\n";
    
    if (code == 204)
        errorBody += "<title>" + printNumber(204) + " " + errorMessage + "</title>\n";
    else
        errorBody += "<title>Error " + printNumber(code) + " " + errorMessage + "</title>\n";
    
    errorBody += "<style>\n";
    errorBody += "body {display: flex; justify-content: center; align-items:center; flex-direction: column; font-family: Arial, sans-serif; margin: 0; padding: 20px;}\n";
    errorBody += "h1 {font-size: 24px;}\n";
    errorBody += "p {font-size: 16px;}\n";
    errorBody += "</style>\n";
    errorBody += "</head>\n";
    errorBody += "<body>\n";
    
    if (code == 204)
        errorBody += "<h1> " + printNumber(code) + " " + errorMessage + "</h1>\n";
    else
        errorBody += "<h1>Error " + printNumber(code) + " " + errorMessage + "</h1>\n";
    
    errorBody += "<p>" + printNumber(code) + "</p>\n";
    errorBody += "</body>\n";
    errorBody += "</html>";
    
    contentTrack = errorBody.size();
    this->head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(errorBody.size()) + "\r\n\r\n";
    this->response = errorBody;
}

void    Response::Response(Request & request, WebServ & server)
{
    this->setResponse(request, server);
    return ;
}

void Response::callErrorPage(Server server, int code) {
    std::string path;
    const std::map<int, std::string> &_error_pages = server.getErrorPageLocation();

    if (code == 301) {
        // Handle redirect separately
        this->head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nLocation: " + this->location + "/" + "\r\n\r\n";
        return;
    }

    if (_error_pages.find(code) != _error_pages.end()) {
        path = _error_pages.find(code)->second;
        this->_error_page = path;
        fd_error.open(path, std::ios::in | std::ios::binary | std::ios::ate);

        if (fd_error.is_open()) {
            contentTrack = fd_error.tellg();
            fd_error.seekg(0, std::ios::beg);

            this->head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(contentTrack) + "\r\n\r\n";
        } else {
            generateErrorPage(code);
        }
    } else {
        generateErrorPage(code);
    }
}

bool checkDirectory(std::string path)
{
	struct stat info;

    if (stat(path.c_str(), &info) != 0)
       return false;
    return S_ISDIR(info.st_mode);
}

bool checkDirectoryEmpty(std::string path)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                closedir (dir);
                return false;
            }
        }
        closedir (dir);
        return true;
    } else {
        return false;
    }
}

void Response::handleDeleteRequest(Request &request, Server &server) {
    std::string path;
    path = request.getUrl();

    //incomplete
    if (access(path.c_str(), F_OK) == -1)
        callErrorPage(server, 404);
    else if (access(path.c_str(), W_OK) == -1)
        callErrorPage(server, 403);
    else if (checkDirectory(path)) {
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

void handleGetRequest(Request request, Server &server) {
    //Ya rbi rak katchouf :)
    std::string path = request.getURL();
    auto it = config_.find(path);
    if (it != config_.end()) {
        // Read the HTML file content
        std::ifstream htmlFile(it->second);
        if (htmlFile.is_open()) {
            std::ostringstream htmlContentStream;
            htmlContentStream << htmlFile.rdbuf();
            return htmlContentStream.str();
        } else {
            // If the file cannot be opened, return a 500 error page
            return callErrorPage(server, 500);
        }
    } else {
        // If the path is not in the configuration, return a 404 error page
        return callErrorPage(server, 404);
    }
    

    // Unsupported method or other cases
    return callErrorPage(server, 500);
}

void Response::handlePostRequest(Request &request, Server &server) {
    //ya rbi rak katchouf
}