# include "Response.hpp"

// Content type
std::string getFileType(const std::string& filePath)
{
    std::map<std::string, std::string> fileExtensions = {
        {".html", "text/html"},
        {".txt", "text/plain"},
        {".pdf", "application/pdf"},
        {".aac", "audio/aac"},
        {".css", "text/css"},
        {".csv", "text/csv"},
        {".doc", "application/msword"},
        {".epub", "application/epub+zip"},
        {".gif", "image/gif"},
        {".htm", "text/html"},
        {".html", "text/html"},
        {".ics", "text/calendar"},
        {".jar", "application/java-archive"},
        {".jpeg", "image/jpeg"},
        {".jpg", "image/jpeg"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".mpeg", "video/mpeg"},
        {".mpkg", "application/vnd.apple.installer+xml"},
        {".odp", "application/vnd.oasis.opendocument.presentation"},
        {".ods", "application/vnd.oasis.opendocument.spreadsheet"},
        {".odt", "application/vnd.oasis.opendocument.text"},
        {".oga", "audio/ogg"},
        {".ogv", "video/ogg"},
        {".ogx", "application/ogg"},
        {".otf", "font/otf"},
        {".png", "image/png"},
        {".pdf", "application/pdf"},
        {".ppt", "application/vnd.ms-powerpoint"},
        {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
        {".rar", "application/x-rar-compressed"},
        {".rtf", "application/rtf"},
        {".sh", "application/x-sh"},
        {".svg", "image/svg+xml"},
        {".swf", "application/x-shockwave-flash"},
        {".tar", "application/x-tar"},
        {".tif", "image/tiff"},
        {".tiff", "image/tiff"},
        {".ts", "application/typescript"},
        {".ttf", "font/ttf"},
        {".vsd", "application/vnd.visio"},
        {".wav", "audio/x-wav"},
        {".weba", "audio/webm"},
        {".webm", "video/webm"},
        {".webp", "image/webp"},
        {".xhtml", "application/xhtml+xml"},
        {".xls", "application/vnd.ms-excel"},
        {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
        {".xml", "application/xml"},
        {".xul", "application/vnd.mozilla.xul+xml"},
        {".zip", "application/zip"},
        {".7z", "application/x-7z-compressed"}
    };

    std::string extension = getExtension(filePath);
    if (!extension.empty()) 
    {
        // Search for the file type in the map
        auto it = fileExtensions.find(extension);
        if (it != fileExtensions.end())
            return it->second; // Return the corresponding file type
    }

    // Default to application/octet-stream if the file type is not recognized
    return "application/octet-stream";
}

// Get extension of file
std::string getExtension(const std::string &path) 
{
    // Find the last dot in the file path
    size_t position = path.rfind('.');
    
    // Check if a dot was found and it's not the last character in the path
    if (position != std::string::npos && position != path.length() - 1) 
        return path.substr(position);
    else
        return "";
}

std::string getExtensionFromType(const std::string &contentType)
{
    std::map<std::string, std::string> contentTypeToExtension = {
        {"text/html", ".html"},
        {"text/plain", ".txt"},
        {"application/pdf", ".pdf"},
        {"audio/aac", ".aac"},
        {"text/css", ".css"},
        {"text/csv", ".csv"},
        {"application/msword", ".doc"},
        {"application/epub+zip", ".epub"},
        {"image/gif", ".gif"},
        {"text/html", ".htm"},
        {"text/html", ".html"},
        {"text/calendar", ".ics"},
        {"application/java-archive", ".jar"},
        {"image/jpeg", ".jpeg"},
        {"image/jpeg", ".jpg"},
        {"application/javascript", ".js"},
        {"application/json", ".json"},
        {"video/mpeg", ".mpeg"},
        {"application/vnd.apple.installer+xml", ".mpkg"},
        {"application/vnd.oasis.opendocument.presentation", ".odp"},
        {"application/vnd.oasis.opendocument.spreadsheet", ".ods"},
        {"application/vnd.oasis.opendocument.text", ".odt"},
        {"audio/ogg", ".oga"},
        {"video/ogg", ".ogv"},
        {"application/ogg", ".ogx"},
        {"font/otf", ".otf"},
        {"image/png", ".png"},
        {"application/pdf", ".pdf"},
        {"application/vnd.ms-powerpoint", ".ppt"},
        {"application/vnd.openxmlformats-officedocument.presentationml.presentation", ".pptx"},
        {"application/x-rar-compressed", ".rar"},
        {"application/rtf", ".rtf"},
        {"application/x-sh", ".sh"},
        {"image/svg+xml", ".svg"},
        {"application/x-shockwave-flash", ".swf"},
        {"application/x-tar", ".tar"},
        {"image/tiff", ".tif"},
        {"image/tiff", ".tiff"},
        {"application/typescript", ".ts"},
        {"font/ttf", ".ttf"},
        {"application/vnd.visio", ".vsd"},
        {"audio/x-wav", ".wav"},
        {"audio/webm", ".weba"},
        {"video/webm", ".webm"},
        {"image/webp", ".webp"},
        {"application/xhtml+xml", ".xhtml"},
        {"application/vnd.ms-excel", ".xls"},
        {"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", ".xlsx"},
        {"application/xml", ".xml"},
        {"application/vnd.mozilla.xul+xml", ".xul"},
        {"application/zip", ".zip"},
        {"application/x-7z-compressed", ".7z"}
    };

    auto it = contentTypeToExtension.find(contentType);
    if (it != contentTypeToExtension.end()) {
        return it->second;
    }

    return "";
}

// Check nature of file requested
bool checkDirectory(std::string path)
{
	struct stat info;

    if (stat(path.c_str(), &info) != 0)
       return false;
    return S_ISDIR(info.st_mode);
}

// Check if directory is empty
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

const  std::map<int, std::string> _status_codes = {
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

std::string Response::statusMessage(int code) 
{
    auto it = _status_codes.find(code);
    if (it != _status_codes.end()) {
        return it->second;
    } else {
        return "Unknown Status";
    }
}

std::string printNumber(int value)
{
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

// Generate error html page
void Response::generateErrorPage(int code)
{
    std::string errorStatus = statusMessage(code);
    std::string errorBody = "<!DOCTYPE html>\n";
    errorBody += "<html>\n";
    errorBody += "<head>\n";
    
    if (code == 204)
        errorBody += "<title>" + printNumber(204) + " " + errorStatus + "</title>\n";
    else
        errorBody += "<title>Error " + printNumber(code) + " " + errorStatus + "</title>\n";
    
    errorBody += "<style>\n";
    errorBody += "body {display: flex; justify-content: center; align-items:center; flex-direction: column; font-family: Arial, sans-serif; margin: 0; padding: 20px;}\n";
    errorBody += "h1 {font-size: 24px;}\n";
    errorBody += "p {font-size: 16px;}\n";
    errorBody += "</style>\n";
    errorBody += "</head>\n";
    errorBody += "<body>\n";
    
    if (code == 204)
        errorBody += "<h1> " + printNumber(code) + " " + errorStatus + "</h1>\n";
    else
        errorBody += "<h1>Error " + printNumber(code) + " " + errorStatus + "</h1>\n";
    
    errorBody += "<p>" + printNumber(code) + "</p>\n";
    errorBody += "</body>\n";
    errorBody += "</html>";
    
    _errorContentLength = errorBody.size();
    this->_head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(errorBody.size()) + "\r\n\r\n";
    this->_response = errorBody;
}

// Check if url matched a conf location
Parser *Response::findLocation(const std::map<std::string, Parser *>& locations, const std::string& path)
{
    auto it = locations.begin();
    
    while (it != locations.end()) {
        this->_locationPath = it->first;

        // If locationPath contains a slash at the end, remove it
        if (!this->_locationPath.empty() && this->_locationPath.back() == '/') {
            this->_locationPath.pop_back();
        }

        if (this->_locationPath == path)
        {
            return it->second; // Match found
        }

        ++it;
    }
    return locations.end()->second; // No match found
}

void Response::handleDirectoryGet(const std::string &directoryPath, HttpRequestParser &request, Parser &server) 
{
    if (!_location->getIndex().empty()) {
        std::vector<std::string>::const_iterator it = this->_location->getIndex().begin();
        while (it != this->_location->getIndex().end()) 
        {
            std::string indexPath = constructFilePath(directoryPath + "/" + *it);
            if (!access(indexPath.c_str(), F_OK))
            {
                handleCgiOrFileGet(request, indexPath, server);
                break ;
            }
            ++it;
        }
    }
    else if (_location->getAutoindex()) {
        listDirectory(directoryPath, request, server);
    } 
    else 
        callErrorPage(server, 403);
}

void Response::handleFileGet(const std::string &filePath, HttpRequestParser &request, Parser &server) 
{
    handleCgiOrFileGet(request, filePath, server);
}

void Response::handleCgiOrFileGet(HttpRequestParser &request, const std::string &path, Parser &server) 
{
    std::map<std::string, std::string>::const_iterator it = server.getCgi().begin();
    std::string fileExtension = getExtension(path);
    while (it != server.getCgi().end()) 
    {
        if ( fileExtension == it->first) 
            handleCgi(request, path, server);
        ++it;
    }
    serveFile(path, server);
}

void Response::handleCgi(HttpRequestParser &request, const std::string &path, Parser &server) 
{
    (void)path;
    (void)server;
    (void)request;
    // CGI cgi =  new CGI(request, this);
    // this->_status_code = cgi->execute();
    // if (this->_status_code == 500)
    //     callErrorPage(server, 500);
    // else if (this->_status_code == 404)
    //     callErrorPage(server, 404);
    // else 
    // {
    //     //this->_head = cgi._responseheaders + "HTTP/1.1 " + printNumber(this->_status_code) + " " + statusMessage(this->_status_code) +
    //     //                 "\r\nConnection: close\r\nContent-Length: " + printNumber(cgi._body.size()) + "\r\n\r\n";
    //     this->_response = cgi._body;
    //     this->_body = cgi._body
    //     this->_contentLength = cgi.body.size();
    // }
}

void Response::renderFile(Parser &server, const std::string &file)
{
    this->_file_path = file;

    this->_file_fd.open(file, std::ios::binary | std::ios::ate);
    if (_file_fd.is_open())
    {
        std::ifstream::pos_type fileSize = _file_fd.tellg();
        _file_fd.seekg(0, std::ios::beg);

        std::vector<char> fileBuffer(fileSize);
        if (_file_fd.read(fileBuffer.data(), fileSize)) {
            this->_head = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                         "Connection: close\r\n"
                         "Content-Type: " + getFileType(file) + "\r\n"
                         "Content-Length: " + printNumber(fileSize) + "\r\n\r\n";
            
            this->_response = std::string(fileBuffer.data(), fileSize);
            this->_errorContentLength = fileSize;
        } else {
            callErrorPage(server, 500); // Internal Server Error
        }

        _file_fd.close();
    } 
    else
        callErrorPage(server, 403); // Forbidden
}

void Response::serveFile(const std::string &filePath, Parser &server) 
{
    std::ifstream fileStream(filePath);
    if (fileStream.good())
        renderFile(server, filePath);
    else if (access(filePath.c_str(), F_OK))
        callErrorPage(server, 404);
    else
        callErrorPage(server, 403);
}

std::string repetetiveSlash(std::string file)
{
    size_t pos = 0;
    while ((pos = file.find("//", pos)) != std::string::npos) {
        file.erase(pos, 1);  // Erase one of the consecutive slashes
    }
    return file;
}

bool endSlash(std::string file)
{
    if (file.back() == '/')
        return 1;
    else
        return 0;
}

std::string createPath(const std::string &path)
{
    // Check if the string is empty
    if (path.empty())
        return "/";

    // Add a slash at the end if it's not already there
    if (path.back() != '/')
        return path + '/';

    return path;
}

std::string Response::constructFilePath(const std::string &path)
{
    std::string file = path;

    if (_location->getslocation() != "/") {
        file.replace(0, _location->getslocation().length(), createPath(_location->getRoot()+ "/"));
    }

    return repetetiveSlash(file);
}

void Response::listDirectory(std::string file, HttpRequestParser &request, Parser &server)
{
    std::string output = "<html><body><ul>";

    DIR *dir = opendir(file.c_str());
    if (dir != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            std::string dirEntryPath;
            if ((this->_location->getslocation() == "/") && (request.getPath() == "/")) {
                dirEntryPath = request.getPath() + ent->d_name;
            } 
            else {
                dirEntryPath = request.getPath() + "/" + ent->d_name;
            }

            output += "<li><a href=\"" + dirEntryPath + "\">" + ent->d_name + "</a></li>";
        }
        closedir(dir);

        output += "</ul></body></html>";
        this->_head = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                             "Connection: close\r\n"
                             "Content-Type: text/html\r\n"
                             "Content-Length: " + printNumber(output.size()) + "\r\n\r\n";

        this->_response = this->_head + output;
        this->_errorContentLength = this->_response.size();
    } 
    else {
        callErrorPage(server, 403);
    }
}

std::string generateName() 
{
    std::string name = "uploads/";
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (int i = 0; i < 5; i++) {
        name += charset[rand() % charset.size()];
    }
    return name;
}

void Response::handleFilePost(HttpRequestParser &request, Parser &server, const std::string &file) 
{
    this->_contentType = request.getHeadersMap().find("Content-Type")->second;  
    std::string name = generateName();
    //this->contentType = getFileType(file);
    std::string filename = name + getExtensionFromType(_contentType);
    std::string uploadDirectory = constructFilePath(_location->getRoot() + "/uploads/");

    // Check if the upload directory exists
    if (access(uploadDirectory.c_str(), F_OK) == -1) {
        callErrorPage(server, 409);
        return;
    }

    std::string filePath = uploadDirectory + filename;
    // Move the uploaded file to the destination
    if (std::rename(filename.c_str(), filePath.c_str()) != 0) {
        std::remove(filename.c_str());
        callErrorPage(server, 500);
        return;
    }

    // Respond with 201 Created and location header
    this->_head = "HTTP/1.1 201 Created\r\nContent-Type: " + getFileType(file) +
           "\r\nContent-Length: 0\r\nLocation: " + constructFilePath(this->_location->getslocation() + "/uploads/") + filename + "\r\n\r\n";
}

void Response::handleDirectoryPost(HttpRequestParser &request, Parser &server, const std::string &file)
{
    // Handle directory requests
    if (endSlash(request.getPath())) {
        if (!this->_location->getIndex().empty())
            handleDirectoryWithIndex(request, server, file);
        else
            callErrorPage(server, 403);
    } 
    else
        callErrorPage(server, 301);
}

void Response::handleDirectoryWithIndex(HttpRequestParser &request, Parser &server, const std::string &file)
{
    (void)request;
    (void)file;
    (void)server;
    // Handle directory requests with index file
    // if (getExtension(request.getPath()) == "php") 
    // {
    //     // Handle CGI for directory
    // } 
    // else
    //     callErrorPage(server, 403);
}

void Response::handleFileUpload(HttpRequestParser &request, Parser &server, const std::string &file) 
{
    (void)request;
    (void)file;
    (void)server;
    // Handle file requests
//     if (getExtension(request.getPath()) == "php") {
//         // Handle CGI for files
//         Cgi cgi(request, file);
//         //....
//     } 
//     else
//         callErrorPage(server, 403);
}