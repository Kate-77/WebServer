# include "Response.hpp"

// Content type
std::string Response::getFileType(const std::string& filePath) {
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

    // Find the last dot in the file path
    size_t position = filePath.rfind('.');
    
    // Check if a dot was found and it's not the last character in the path
    if (position != std::string::npos && position != filePath.length() - 1) {
        // Extract the file extension
        std::string extension = filePath.substr(position);

        // Search for the file type in the map
        auto it = fileExtensions.find(extension);
        if (it != fileExtensions.end()) {
            return it->second; // Return the corresponding file type
        }
    }

    // Default to application/octet-stream if the file type is not recognized
    return "application/octet-stream";
}

// check nature of file requested
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

// Generate error html page
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