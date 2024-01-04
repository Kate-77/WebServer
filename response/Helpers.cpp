# include "Response.hpp"

// Content type
std::string getFileType(const std::string& filePath)
{
    std::map<std::string, std::string> fileExtensions;
        fileExtensions[".html"] = "text/html";
        fileExtensions[".txt"] = "text/plain";
        fileExtensions[".pdf"] = "application/pdf";
        fileExtensions[".aac"] = "audio/aac";
        fileExtensions[".css"] = "text/css";
        fileExtensions[".csv"] = "text/csv";
        fileExtensions[".doc"] = "application/msword";
        fileExtensions[".epub"] = "application/epub+zip";
        fileExtensions[".gif"] = "image/gif";
        fileExtensions[".htm"] = "text/html";
        fileExtensions[".html"] = "text/html";
        fileExtensions[".ics"] = "text/calendar";
        fileExtensions[".jar"] = "application/java-archive";
        fileExtensions[".jpeg"] = "image/jpeg";
        fileExtensions[".jpg"] = "image/jpeg";
        fileExtensions[".js"] = "application/javascript";
        fileExtensions[".json"] = "application/json";
        fileExtensions[".mpeg"] = "video/mpeg";
        fileExtensions[".mpkg"] = "application/vnd.apple.installer+xml";
        fileExtensions[".odp"] = "application/vnd.oasis.opendocument.presentation";
        fileExtensions[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        fileExtensions[".odt"] = "application/vnd.oasis.opendocument.text";
        fileExtensions[".oga"] = "audio/ogg";
        fileExtensions[".ogv"] = "video/ogg";
        fileExtensions[".ogx"] = "application/ogg";
        fileExtensions[".otf"] = "font/otf";
        fileExtensions[".png"] = "image/png";
        fileExtensions[".pdf"] = "application/pdf";
        fileExtensions[".ppt"] = "application/vnd.ms-powerpoint";
        fileExtensions[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        fileExtensions[".rar"] = "application/x-rar-compressed";
        fileExtensions[".rtf"] = "application/rtf";
        fileExtensions[".sh"] = "application/x-sh";
        fileExtensions[".svg"] = "image/svg+xml";
        fileExtensions[".swf"] = "application/x-shockwave-flash";
        fileExtensions[".tar"] = "application/x-tar";
        fileExtensions[".tif"] = "image/tiff";
        fileExtensions[".tiff"] = "image/tiff";
        fileExtensions[".ts"] = "application/typescript";
        fileExtensions[".ttf"] = "font/ttf";
        fileExtensions[".vsd"] = "application/vnd.visio";
        fileExtensions[".wav"] = "audio/x-wav";
        fileExtensions[".weba"] = "audio/webm";
        fileExtensions[".webm"] = "video/webm";
        fileExtensions[".webp"] = "image/webp";
        fileExtensions[".xhtml"] = "application/xhtml+xml";
        fileExtensions[".xls"] = "application/vnd.ms-excel";
        fileExtensions[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        fileExtensions[".xml"] = "application/xml";
        fileExtensions[".xul"] = "application/vnd.mozilla.xul+xml";
        fileExtensions[".zip"] = "application/zip";
        fileExtensions[".7z"] = "application/x-7z-compressed";


    std::string extension = getExtension(filePath);
    if (!extension.empty()) 
    {
        // Search for the file type in the map
        std::map<std::string, std::string>::const_iterator it = fileExtensions.find(extension);
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
    std::map<std::string, std::string> contentTypeToExtension;
        contentTypeToExtension["text/html"] = ".html";
        contentTypeToExtension["text/plain"] = ".txt";
        contentTypeToExtension["application/pdf"] = ".pdf";
        contentTypeToExtension["audio/aac"] = ".aac";
        contentTypeToExtension["text/css"] = ".css";
        contentTypeToExtension["text/csv"] = ".csv";
        contentTypeToExtension["application/msword"] = ".doc";
        contentTypeToExtension["application/epub+zip"] = ".epub";
        contentTypeToExtension["image/gif"] = ".gif";
        contentTypeToExtension["text/html"] = ".htm";
        contentTypeToExtension["text/html"] = ".html";
        contentTypeToExtension["text/calendar"] = ".ics";
        contentTypeToExtension["application/java-archive"] = ".jar";
        contentTypeToExtension["image/jpeg"] = ".jpeg";
        contentTypeToExtension["image/jpeg"] = ".jpg";
        contentTypeToExtension["application/javascript"] = ".js";
        contentTypeToExtension["application/json"] = ".json";
        contentTypeToExtension["video/mpeg"] = ".mpeg";
        contentTypeToExtension["application/vnd.apple.installer+xml"] = ".mpkg";
        contentTypeToExtension["application/vnd.oasis.opendocument.presentation"] = ".odp";
        contentTypeToExtension["application/vnd.oasis.opendocument.spreadsheet"] = ".ods";
        contentTypeToExtension["application/vnd.oasis.opendocument.text"] = ".odt";
        contentTypeToExtension["audio/ogg"] = ".oga";
        contentTypeToExtension["video/ogg"] = ".ogv";
        contentTypeToExtension["application/ogg"] = ".ogx";
        contentTypeToExtension["font/otf"] = ".otf";
        contentTypeToExtension["image/png"] = ".png";
        contentTypeToExtension["application/pdf"] = ".pdf";
        contentTypeToExtension["application/vnd.ms-powerpoint"] = ".ppt";
        contentTypeToExtension["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = ".pptx";
        contentTypeToExtension["application/x-rar-compressed"] = ".rar";
        contentTypeToExtension["application/rtf"] = ".rtf";
        contentTypeToExtension["application/x-sh"] = ".sh";
        contentTypeToExtension["image/svg+xml"] = ".svg";
        contentTypeToExtension["application/x-shockwave-flash"] = ".swf";
        contentTypeToExtension["application/x-tar"] = ".tar";
        contentTypeToExtension["image/tiff"] = ".tif";
        contentTypeToExtension["image/tiff"] = ".tiff";
        contentTypeToExtension["application/typescript"] = ".ts";
        contentTypeToExtension["font/ttf"] = ".ttf";
        contentTypeToExtension["application/vnd.visio"] = ".vsd";
        contentTypeToExtension["audio/x-wav"] = ".wav";
        contentTypeToExtension["audio/webm"] = ".weba";
        contentTypeToExtension["video/webm"] = ".webm";
        contentTypeToExtension["image/webp"] = ".webp";
        contentTypeToExtension["application/xhtml+xml"] = ".xhtml";
        contentTypeToExtension["application/vnd.ms-excel"] = ".xls";
        contentTypeToExtension["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = ".xlsx";
        contentTypeToExtension["application/xml"] = ".xml";
        contentTypeToExtension["application/vnd.mozilla.xul+xml"] = ".xul";
        contentTypeToExtension["application/zip"] = ".zip";
        contentTypeToExtension["application/x-7z-compressed"] = ".7z";

     std::map<std::string, std::string>::const_iterator it = contentTypeToExtension.find(contentType);
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

std::string Response::statusMessage(int code) 
{
    this->_status_codes[100] = "Continue";
    this->_status_codes[101] = "Switching Protocols";
    this->_status_codes[102] = "Processing";
    this->_status_codes[200] = "OK";
    this->_status_codes[201] = "Created";
    this->_status_codes[202] = "Accepted";
    this->_status_codes[203] = "Non-Authoritative Information";
    this->_status_codes[204] = "No Content";
    this->_status_codes[205] = "Reset Content";
    this->_status_codes[206] = "Partial Content";
    this->_status_codes[207] = "Multi-Status";
    this->_status_codes[208] = "Already Reported";
    this->_status_codes[226] = "IM Used";
    this->_status_codes[300] = "Multiple Choices";
    this->_status_codes[301] = "Moved Permanently";
    this->_status_codes[302] = "Found";
    this->_status_codes[303] = "See Other";
    this->_status_codes[304] = "Not Modified";
    this->_status_codes[305] = "Use Proxy";
    this->_status_codes[306] = "Switch Proxy";
    this->_status_codes[307] = "Temporary Redirect";
    this->_status_codes[308] = "Permanent Redirect";
    this->_status_codes[400] = "Bad Request";
    this->_status_codes[401] = "Unauthorized";
    this->_status_codes[402] = "Payment Required";
    this->_status_codes[403] = "Forbidden";
    this->_status_codes[404] = "Not Found";
    this->_status_codes[405] = "Method Not Allowed";
    this->_status_codes[406] = "Not Acceptable";
    this->_status_codes[407] = "Proxy Authentication Required";
    this->_status_codes[408] = "Request Timeout";
    this->_status_codes[409] = "Conflict";
    this->_status_codes[410] = "Gone";
    this->_status_codes[411] = "Length Required";
    this->_status_codes[412] = "Precondition Failed";
    this->_status_codes[413] = "Payload Too Large";
    this->_status_codes[414] = "URI Too Long";
    this->_status_codes[415] = "Unsupported Media Type";
    this->_status_codes[416] = "Range Not Satisfiable";
    this->_status_codes[417] = "Expectation Failed";
    this->_status_codes[418] = "I'm a teapot";
    this->_status_codes[421] = "Misdirected Request";
    this->_status_codes[422] = "Unprocessable Entity";
    this->_status_codes[423] = "Locked";
    this->_status_codes[424] = "Failed Dependency";
    this->_status_codes[425] = "Too Early";
    this->_status_codes[426] = "Upgrade Required";
    this->_status_codes[428] = "Precondition Required";
    this->_status_codes[429] = "Too Many Requests";
    this->_status_codes[431] = "Request Header Fields Too Large";
    this->_status_codes[451] = "Unavailable For Legal Reasons";
    this->_status_codes[500] = "Internal Server Error";
    this->_status_codes[501] = "Not Implemented";
    this->_status_codes[502] = "Bad Gateway";
    this->_status_codes[503] = "Service Unavailable";
    this->_status_codes[504] = "Gateway Timeout";
    this->_status_codes[505] = "HTTP Version Not Supported";
    this->_status_codes[506] = "Variant Also Negotiates";
    this->_status_codes[507] = "Insufficient Storage";
    this->_status_codes[508] = "Loop Detected";
    this->_status_codes[510] = "Not Extended";
    this->_status_codes[511] = "Network Authentication Required";

    std::map<int, std::string>::const_iterator it = _status_codes.find(code);
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
    std::map<std::string, Parser *>::const_iterator it = locations.begin();
    
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
        {
            std::map<std::string, std::string>::const_iterator it1 = server.getCgi().find(it->first);
            if (it1 != server.getCgi().end())
                this->_cgi_bin = it1->second;
            handleCgi(request, path, server);
        }
        ++it;
    }
    serveFile(path, server);
}

void Response::handleCgi(HttpRequestParser &request, const std::string &path, Parser &server) 
{
    std::string key, value; //split the key and the value
    std::map<std::string, std::string>  cgi_headers; //create map headers
    // const std::string   end_of_file = "\r\n"; //seperator
    std::istringstream a(this->_head);
    std::string line;
    while(std::getline(a, line))
    {
        std::istringstream b(line);
        std::getline(b, key, ':'); //key
        std::getline(b, value); //value
        std::cout << "key: " << key << "value: " << value << std::endl;
        cgi_headers[key] = value; // assign
    }
    this->_file = path;
    // (void)server;
    // (void)request;
    CGI cgi = CGI(request, *this);
    this->_status_code = cgi.execute();
    if (this->_status_code == 500)
        callErrorPage(server, 500);
    else if (this->_status_code == 404)
        callErrorPage(server, 404);
    else 
        cgi.parseHeadersAndBody(this->_head, this->_body);
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
        } 
        else
            callErrorPage(server, 500); // Internal Server Error

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

    this->_file_path = uploadDirectory + filename;
    // Move the uploaded file to the destination
    if (std::rename(filename.c_str(), this->_file_path.c_str()) != 0) {
        std::remove(filename.c_str());
    }

    // Respond with 201 Created and location header
    this->_head = "HTTP/1.1 201 Created\r\nContent-Type: " + getFileType(file) +
           "\r\nContent-Length: 0\r\nLocation: " + this->_file_path + "\r\n\r\n";
}

void Response::handleDirectoryPost(HttpRequestParser &request, Parser &server, const std::string &file)
{
    // Handle directory requests
    if (endSlash(request.getPath())) {
        if (!this->_location->getIndex().empty())
            handleDirFile(request, server, file);
        else
            callErrorPage(server, 403);
    } 
    else
        callErrorPage(server, 301);
}

void Response::handleDirFile(HttpRequestParser &request, Parser &server, const std::string &path)
{
    // Handle directory requests with index file
    std::map<std::string, std::string>::const_iterator it = server.getCgi().begin();
    std::string fileExtension = getExtension(path);
    bool cgi = 0;
    while (it != server.getCgi().end()) 
    {
        if ( fileExtension == it->first)
        {
            std::map<std::string, std::string>::const_iterator it1 = server.getCgi().find(it->first);
            if (it1 != server.getCgi().end())
                this->_cgi_bin = it1->second;
            handleCgi(request, path, server);
            cgi = 1;
        }
        ++it;
    }
    if (!cgi)
        callErrorPage(server, 404);
}