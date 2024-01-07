# include "Response.hpp"

// Generate error html page
void Response::generateErrorPage(int code)
{
	printf("generate error page start\n");
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
    this->_contentLength = errorBody.size();
}

// Check if url matched a conf location
int Response::findLocation(Parser &server, const std::string& path)
{
    std::map<std::string, Parser *> locations = server.getLocation();
    std::map<std::string, Parser *>::reverse_iterator it = locations.rbegin();
    
    while (it != locations.rend()) {
        this->_locationPath = it->first;

        // If locationPath contains a slash at the end, remove it
        // if (!this->_locationPath.empty() && this->_locationPath.back() == '/') {
        //     this->_locationPath.pop_back();
        // }
        if (path.compare(0, _locationPath.length(), _locationPath) == 0)
        {
            this->_location = it->second;
            return 42; // Match found
        }
        ++it;
    }
    return -1; // No match found
}

// Get Method for directory case
void Response::handleDirectoryGet(const std::string &directoryPath, HttpRequestParser &request, Parser &server) 
{
    if (!_location->getIndex().empty()) {
        std::vector<std::string>::const_iterator it = this->_location->getIndex().begin();
        while (it != this->_location->getIndex().end()) 
        {
            std::string indexPath = repetetiveSlash(directoryPath + "/" + *it);
            if (!access(indexPath.c_str(), F_OK))
            {
                handleCgiOrFileGet(request, indexPath, server);
                break ;
            }
			else
				callErrorPage(server, 404);
            ++it;
        }
    }
    else if (_location->getAutoindex()) {
        listDirectory(directoryPath, request, server);
    } 
    else 
        callErrorPage(server, 403);
}

// Get Method for file case
void Response::handleFileGet(const std::string &filePath, HttpRequestParser &request, Parser &server) 
{
    handleCgiOrFileGet(request, filePath, server);
}

// Get Method (file?cgi:normal)
void Response::handleCgiOrFileGet(HttpRequestParser &request, const std::string &path, Parser &server) 
{
    std::map<std::string, std::string> pp = _location->getCgi();
    std::map<std::string, std::string>::const_iterator it = pp.begin();
    std::string fileExtension = getExtension(path);
	int	cgi = 0;
    while (it != pp.end()) 
    {
        if ( fileExtension == it->first)
        {
            std::map<std::string, std::string> mm =_location->getCgi();
            std::map<std::string, std::string>::const_iterator it1 = mm.find(it->first);
            if (it1 != mm.end())
                this->_cgi_bin = it1->second;
            handleCgi(request, path, server);
			cgi = 1;
        }
        ++it;
    }
    if (!cgi)
    	serveFile(path, server);
}


int Response::handleCgi(HttpRequestParser &request, const std::string &path, Parser &server) 
{
    std::string key, value; //split the key and the value
    const std::string   end_of_file = "\r\n"; //seperator
    this->_file = path;
    this->_head = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                "Connection: close\r\n"
                "Content-Type: " + getFileType(_file) + "\r\n"
                "Content-Length: 0" + "\r\n\r\n";
    std::map<std::string, std::string>  cgi_headers; //create map headers
    // serveFile(_file, server);
    std::istringstream a(this->_head);
    std::string line;
    while(std::getline(a, line))
    {
        std::istringstream b(line);
        // std::cout << "line: |" << line << std::endl;
        if(line.find(':') == std::string::npos || line.find(':') == 0)
        {
            std::getline(b, key, ' '); //key
            std::getline(b, value); //value
            cgi_headers[key] = value;
            continue ;
        }
        std::getline(b, key, ':'); //key
        std::getline(b, value); //value
        cgi_headers[key] = value; // assign
    }
    
    CGI cgi = CGI(request, *this);
    this->_status_code = cgi.execute();
    if (this->_status_code == 500)
        callErrorPage(server, 500);
    else if (this->_status_code == 502)
        callErrorPage(server, 502);
    else
    {
        cgi.parseHeadersAndBody(cgi_headers, this->_response);
        this->_head.erase();
        for(std::map<std::string, std::string>::iterator it = cgi_headers.end(); it != cgi_headers.begin(); it--)
        {
            if(it->first.length() > 30 || it->first.empty() == true)
                continue;
            if(it->first == "Content-Length")
                this->_contentLength = this->_response.length();
            // //printf("#FIRST : %s |%lu | #SECOND %s\n",it->first.c_str(), it->first.length(), it->second.c_str());
            if(it->first == "HTTP/1.1")
                {
                    // std::cout << "here " << std::endl;
                    this->_head += it->first  + " " + it->second + end_of_file;
                    continue ;
                }
            this->_head += it->first + ": " + it->second + end_of_file;
            // //printf("houna: %s\n", this->_head.c_str());
        }
        std::cout << "final header:|" << this->_head << "|" << std::endl;
        std::cout << "final body: |" << this->_response << "|" << std::endl;
        this->_head += "\r\n\r\n";
    }
    return (1);
}

void Response::renderFile(Parser &server, const std::string &file)
{
    this->_file_path = file;
    // //printf("jiti?\n");
	this->_file_fd.open(file, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (_file_fd.is_open())
    {
        std::ifstream::pos_type fileSize = _file_fd.tellg();
        _file_fd.seekg(0, std::ios::beg);
        this->_contentLength = fileSize;
        this->_errorContentLength = fileSize;
        this->_head = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                    "Connection: close\r\n"
                    "Content-Type: " + getFileType(_file_path) + "\r\n"
                    "Content-Length: " + printNumber(_contentLength) + "\r\n\r\n";
        _file_fd.close();
    } 
    else
        callErrorPage(server, 403); // Forbidden
}

void Response::serveFile(const std::string &filePath, Parser &server) 
{
	std::cout << "serveFile: " << filePath << std::endl;
    std::ifstream fileStream(filePath);
    if (fileStream.good() && !access(filePath.c_str(), F_OK))
        renderFile(server, filePath);
    else if (access(filePath.c_str(), F_OK))
        callErrorPage(server, 404);
    else if (access(filePath.c_str(), R_OK))
        callErrorPage(server, 401);
    else
        callErrorPage(server, 403);
}

// Get Method listing directory in autoindex case
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
            // //printf("chno dirPath? : %s\n", dirEntryPath.c_str());

            output += "<li><a href=\"" + dirEntryPath + "\">" + ent->d_name + "</a></li>";
        }
        closedir(dir);

        output += "</ul></body></html>";
        this->_head = "HTTP/1.1 " + printNumber(200) + " OK\r\n"
                        "Connection: close\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + printNumber(output.size()) + "\r\n\r\n";

        this->_response = output;
        this->_errorContentLength = this->_response.size();
        this->_contentLength = this->_response.size();
    } 
    else {
        callErrorPage(server, 403);
    }
}

void Response::handleFilePost(HttpRequestParser &request, Parser &server, const std::string &file) 
{
    (void) file;
    struct stat status;
    this->_contentType = request.getHeadersMap().find("Content-Type")->second;
	this->_post_file_name = request.getBodyFileName();
    std::string name = generateName();
    this->_file_path = name + getExtensionFromType(_contentType);
    std::string uploadDirectory = repetetiveSlash(_location->getRoot() + "/uploads/");

    // Check if the upload directory exists
    if (access(uploadDirectory.c_str(), F_OK)) {
        callErrorPage(server, 404);
        return;
    }

    // Move the uploaded file to the destination
    else if (std::rename(this->_post_file_name.c_str(), this->_file_path.c_str()) != 0)
        std::remove(this->_post_file_name.c_str());

    // Create header
    stat(_file_path.c_str(), &status);
    this->_contentLength = status.st_size;
    this->_errorContentLength = status.st_size;
    this->_status_code = 201;
    this->_head = "HTTP/1.1 " + printNumber(this->_status_code) + " Created"
                + "\r\nConnection: close"
                + "\r\nLocation: " + this->_locationPath  + this->_file_path
                + "\r\nContent-Type: " + this->_contentType
                + "\r\nContent-Length: " + printNumber(_contentLength) + "\r\n\r\n";
}

void Response::handleDirectoryPost(HttpRequestParser &request, Parser &server, const std::string &file)
{
    // Handle directory requests
    if (endSlash(request.getPath()) && request.getPath().size() != 1) {
        // //printf("bayna hna\n");
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

	// std::map<std::string, std::string> ee;
    // std::map<std::string, std::string>::const_iterator it = ee.begin();
    // std::string fileExtension = getExtension(path);
    int cgi = 0;
    std::map<std::string, std::string> pp = _location->getCgi();
    std::map<std::string, std::string>::const_iterator it = pp.begin();
    std::string fileExtension = getExtension(path);
    while (it != pp.end()) 
    {
        if ( fileExtension == it->first)
        {
			std::map<std::string, std::string> mm =_location->getCgi();
            std::map<std::string, std::string>::const_iterator it1 = mm.find(it->first);
            if (it1 != mm.end())
                this->_cgi_bin = it1->second;
            cgi = handleCgi(request, path, server);
            break ;
        }
        ++it;
    }
    //printf("hanta jiti ! cgi: %d\n", cgi);
    if (!cgi)
        callErrorPage(server, 403);
}

std::string & Response::lefttrim(std::string & s, const char * t = " \t\n\r\f\v") 
{
  std::string::size_type index = s.find_first_not_of(t);
  if (std::string::npos == index) 
  {
    return (s);
  }
  s.erase(0, index);
  return (s);
}

std::string & Response::righttrim(std::string & s, const char * t = " \t\n\r\f\v") 
{
  std::string::size_type index = s.find_last_not_of(t);
  if (std::string::npos == index) 
  {
    return (s);
  }
  s.erase(index + 1);
  return (s);
}

// trim from left & right
std::string & Response::trim(std::string & s, const char * t = " \t\n\r\f\v") 
{
  return (lefttrim(righttrim(s, t), t));
}