# include "Response.hpp"

// Content type
std::string getFileType(const std::string& filePath)
{
    std::unordered_map<std::string, std::string> fileExtensions = {
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
    std::unordered_map<std::string, std::string> contentTypeToExtension = {
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
        {"application/vnd.apple.installer+xml". ".mpkg"},
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

std::string Response::statusMessage(int code) 
{
    auto it = _status_code.find(code);
    if (it != _status_code.end()) {
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
    
    contentTrack = errorBody.size();
    this->head = "HTTP/1.1 " + printNumber(code) + " " + statusMessage(code) + "\r\nContent-Type: text/html\r\nContent-Length: " + printNumber(errorBody.size()) + "\r\n\r\n";
    this->response = errorBody;
}

// Check if url matched a conf location
Parser *Response::findLocation(const std::map<std::string, Parser *>& locations, const std::string& path)
{
    auto it = locations.begin();
    
    while (it != locations.end()) {
        std::string locationPath = it->first; or it->second->getslocation();

        // If locationPath contains a slash at the end, remove it
        if (!locationPath.empty() && locationPath.back() == '/') {
            locationPath.pop_back();
        }

        if (locationPath == path)
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
        std::string indexPath = constructFilePath(directoryPath + "/" + _location->getIndex());
        handleCgiOrFile(request, indexPath, server);
    } 
    else if (_location->getAutoindex()) {
        listDirectory(directoryPath, request, server);
    } 
    else 
        callErrorPage(server, 403);
}

void Response::handleFileGet(const std::string &filePath, Request &request, Server_storage &server) 
{
    handleCgiOrFile(request, filePath, server);
}

void Response::handleCgiOrFileGet(Request &request, const std::string &path, Server_storage &server) 
{
    // CGI HANDLER
}

void Response::handleCgi(Request &request, const std::string &cgiPath, Server_storage &server) 
{
    // CGI HANDLER
}

void Response::renderFile(Parser &server, const std::string &file)
{
    std::ifstream fileStream(file, std::ios::binary | std::ios::ate);
    
    if (fileStream.is_open()) {
        std::ifstream::pos_type fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        std::vector<char> fileBuffer(fileSize);
        if (fileStream.read(fileBuffer.data(), fileSize)) {
            this->head = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                         "Connection: close\r\n"
                         "Content-Type: " + getFileType(file) + "\r\n"
                         "Content-Length: " + printNumber(fileSize) + "\r\n\r\n";
            
            this->response = std::string(fileBuffer.data(), fileSize);
            this->contentTrack = fileSize;
        } else {
            callErrorPage(server, 500); // Internal Server Error
        }

        fileStream.close();
    } 
    else
        callErrorPage(server, 403); // Forbidden
}

void Response::serveFile(const std::string &filePath, Server_storage &server) 
{
    std::ifstream fileStream(filePath);
    if (fileStream.good())
    {
        renderFile(server, filePath);
    }
    else if (access(filePath.c_str(), F_OK))
    {
        callErrorPage(server, 404);
    } 
    else {
        callErrorPage(server, 403);
    }
}

std::string repetetiveSlash(std::string file)
{
    size_t pos = 0;
    while ((pos = file.find("//", pos)) != std::string::npos) {
        file.erase(pos, 1);  // Erase one of the consecutive slashes
    }
    return file;
}

static bool endSlash(std::string file)
{
    if (file.back() == '/')
        return 1;
    else
        return 0;
}

std::string createPath(const std::string &path)
{
    // Check if the string is empty
    if (path.empty()) {
        return "/";
    }

    // Add a slash at the end if it's not already there
    if (path.back() != '/') {
        return path + '/';
    }

    return result;
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
            if (this->_location->getslocation()) == "/" && request.getUrl() == "/") {
                dirEntryPath = request.getUrl() + ent->d_name;
            } 
            else {
                dirEntryPath = request.getUrl() + "/" + ent->d_name;
            }

            output += "<li><a href=\"" + dirEntryPath + "\">" + ent->d_name + "</a></li>";
        }
        closedir(dir);

        output += "</ul></body></html>";
        std::string header = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                             "Connection: close\r\n"
                             "Content-Type: text/html\r\n"
                             "Content-Length: " + printNumber(output.size()) + "\r\n\r\n";

        this->response = header + output;
        this->contentTrack = response.size();
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
    this->contentType = request.headers_map.find("Content-Type")->second;  
    std::string name = generateName();
    //this->contentType = getFileType(file);
    std::string filename = name + getExtensionFromType(contentType);
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
           "\r\nContent-Length: 0\r\nLocation: " + constructFilePath(this->_locations->getslocation() + "/uploads/") + filename + "\r\n\r\n";
}

void Response::handleDirectoryPost(HttpRequestParser &request, Parser &server, const std::string &file)
{
    // Handle directory requests
    if (endSlash(request.getUrl())) {
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
    // Handle directory requests with index file
    if (getExtension(request.getUrl()) == "php") 
    {
        // Handle CGI for directory
    } 
    else
        callErrorPage(server, 403);
}

void Response::handleFileUpload(HttpRequestParser &request, Parser &server, const std::string &file) 
{
    // Handle file requests
    if (getExtension(request.getUrl()) == "php") {
        // Handle CGI for files
        Cgi cgi(request, file);
        //....
    } 
    else
        callErrorPage(server, 403);
}