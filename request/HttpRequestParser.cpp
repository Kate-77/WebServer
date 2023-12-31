#include "HttpRequestParser.hpp"
#include <fstream>

HttpRequestParser::HttpRequestParser() {
	status_code_ = -1;
	method = "";
	path = "";
	query = "";
	version = "";
	bodyFileName = "/var/tmp/file_" + generateName();
}

HttpRequestParser::~HttpRequestParser() {}

HttpRequestParser::HttpRequestParser(const HttpRequestParser &http_request_parser) {
	*this = http_request_parser;
}

HttpRequestParser &HttpRequestParser::operator=(const HttpRequestParser &http_request_parser) {
	this->status_code_ = http_request_parser.status_code_;
	this->method = http_request_parser.method;
	return *this;
}

int HttpRequestParser::getStatusCode() const {
	return status_code_;
}

std::string HttpRequestParser::getMethod() const {
	return method;
}

std::string HttpRequestParser::getPath() const {
	return path;;
}

std::string HttpRequestParser::getQuery() const {
	return query;
}

std::string HttpRequestParser::getVersion() const {
	return version;
}

std::string HttpRequestParser::getBodyFileName() const
{
	return bodyFileName;
}

int HttpRequestParser::getBodyFileFD() const
{
	return bodyFileFD;
}

void HttpRequestParser::setStatusCode(int status_code) {
	this->status_code_ = status_code;
}

void HttpRequestParser::setMethod(std::string method) {
	this->method = method;
}

void HttpRequestParser::setPath(std::string path) {
	this->path = path;
}

void HttpRequestParser::setQuery(std::string query) {
	this->query = query;
}

void HttpRequestParser::setVersion(std::string version) {
	this->version = version;
}

void HttpRequestParser::setBodyFileName(std::string bodyFileName) {
	this->bodyFileName = bodyFileName;
}

void HttpRequestParser::setBodyFileFD(int bodyFileFD) {
	this->bodyFileFD = bodyFileFD;
}

void HttpRequestParser::parseRequest(ssize_t nbytes, unsigned char *buf, int &Done)
{
	std::string reqLine_Headers;
	size_t reqLine_Headers_Length;
	unsigned char* bodyStart = nullptr;
	size_t bodyLength = 0;

	buf[nbytes] = '\0';

	// Find the empty request_line that separates headers and body
	const char* doubleCRLF = "\r\n\r\n";
	unsigned char* bodyStartPtr = static_cast<unsigned char*>(memmem(buf, nbytes, doubleCRLF, strlen(doubleCRLF)));
	
	if (bodyStartPtr != NULL)
	{
		reqLine_Headers_Length = bodyStartPtr - buf + 2;
        bodyStart = bodyStartPtr + strlen(doubleCRLF);
        reqLine_Headers.assign(reinterpret_cast<char *>(buf), reqLine_Headers_Length);
	}
	else
		throw 400;
   	parseRequestLine_Headers(reqLine_Headers);

	//print headers_map

	for(std::map<std::string, std::string>::iterator it = headers_map.begin(); it != headers_map.end(); it++)
	{
		std::cout << "key = " << it->first << " value = " << it->second << std::endl;
	}


	
	
	if (getMethod() == "POST")
	{
        bodyLength = nbytes - reqLine_Headers_Length - strlen(doubleCRLF);
		// parseBody(bodyStart, bodyLength);
	}
	else
		Done = 1;
	std::cout << "bodyLength =************ " << bodyLength << std::endl;
	std::cout << "bodyStart = " << bodyStart << std::endl;
}

void HttpRequestParser::parseRequestLine_Headers(std::string reqLine_Headers)
{
	std::istringstream iss(reqLine_Headers);
	std::string request_line;

	std::getline(iss, request_line);

	// Parse the request request_line
	parse_request_line(request_line);
	printf("request_line = %s\n", request_line.c_str());
	printf("method = %s\n", getMethod().c_str());
	printf("path = %s\n", getPath().c_str());
	printf("query = %s\n", getQuery().c_str());
	printf("version = %s\n", getVersion().c_str());

	// Parse the headers
	// printf("headers = %s\n", iss.str().c_str());
	parse_headers(iss);
}

// void HttpRequestParser::parseBody(unsigned char *bodyStart, size_t bodyLength)
// {
// }

void HttpRequestParser::parse_request_line(std::string request_line)
{
	std::istringstream iss(request_line);
	std::string uri;
	std::string empty;

	if (iss.fail())
		throw 400;
	iss >> this->method;
	iss >> uri;

	std::istringstream tmp_path(uri);
	getline(tmp_path, uri, '?');
	this->path = uri;
	if (this->path.size() > 2048)
		throw 414;
	tmp_path >> this->query;

	iss >> this->version;
	iss >> empty;
	if (!empty.empty())
		throw 400;
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
		throw 501;

	if (this->version != "HTTP/1.1")
		throw 505;
	if (this->method == "POST")
	{
		this->bodyFileFD = open(this->bodyFileName.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
	}
}

std::string HttpRequestParser::generateName()
{
	std::string name;
	static int i = 0;
	std::stringstream ss;
	ss << i;
	name = ss.str();
	i++;
	return name;
}

void HttpRequestParser::parse_headers(std::istringstream &headers)
{
	std::string header_line;
	std::string key;
	std::string value;

	while (std::getline(headers, header_line))
	{
		// printf("header_line = %s\n", header_line.c_str());

		size_t pos = header_line.find(": ");
		if (pos == std::string::npos)
			throw 400;
		else
		{
			key = header_line.substr(0, pos);
			header_line.erase(0, pos + 2);
		}
		// printf("key = [%s]\n", key.c_str());
		pos = header_line.find("\r");
		if (pos == std::string::npos)
			throw 400;
		else
		{
			value = header_line.substr(0, pos);
		}
		// printf("value = [%s]\n", value.c_str());
		headers_map.insert(std::pair<std::string, std::string>(key, value));
	}
}
