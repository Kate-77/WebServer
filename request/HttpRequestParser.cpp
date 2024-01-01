#include "HttpRequestParser.hpp"
#include <fstream>

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

HttpRequestParser::HttpRequestParser() {
	status_code_ = -1;
	method = "";
	path = "";
	query = "";
	version = "";
	bodyFileName = "/Users/zowa/Desktop/" + generateName();
	bodyFileFD = -1;
}

HttpRequestParser::~HttpRequestParser() {}

HttpRequestParser::HttpRequestParser(const HttpRequestParser &http_request_parser) {
	*this = http_request_parser;
}

HttpRequestParser &HttpRequestParser::operator=(const HttpRequestParser &http_request_parser) {
	this->status_code_ = http_request_parser.status_code_;
	this->method = http_request_parser.method;
	this->path = http_request_parser.path;
	this->query = http_request_parser.query;
	this->version = http_request_parser.version;
	this->bodyFileName = http_request_parser.bodyFileName;
	this->bodyFileFD = http_request_parser.bodyFileFD;
	this->headers_map = http_request_parser.headers_map;
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

std::map<std::string, std::string> HttpRequestParser::getHeadersMap() const {
	return headers_map;
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

void HttpRequestParser::setHeadersMap(std::map<std::string, std::string> headers_map)
{
	this->headers_map = headers_map;
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


	if (getMethod() == "POST")
	{
        bodyLength = nbytes - reqLine_Headers_Length - 2;
		parse_body(bodyStart, bodyLength, Done);
	}
	else
		Done = 1;
}

void HttpRequestParser::parseRequestLine_Headers(std::string reqLine_Headers)
{
	std::istringstream iss(reqLine_Headers);
	std::string request_line;

	std::getline(iss, request_line);

	// Parse the request request_line
	parse_request_line(request_line);

	// Parse the headers
	parse_headers(iss);
}

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
		if (this->bodyFileFD == -1)
			throw 500;
	}
}

void HttpRequestParser::parse_headers(std::istringstream &headers)
{
	std::string header_line;
	std::string key;
	std::string value;

	while (std::getline(headers, header_line))
	{
		size_t pos = header_line.find(": ");
		if (pos == std::string::npos)
			throw 400;
		else
		{
			key = header_line.substr(0, pos);
			header_line.erase(0, pos + 2);
		}
		pos = header_line.find("\r");
		if (pos == std::string::npos)
			throw 400;
		else
		{
			value = header_line.substr(0, pos);
		}

		if (key == "Content-Length")
		{
			if (value.empty())
                throw 400;
            if (value.find_first_not_of("0123456789") != std::string::npos)
                throw 400;
        }

		if (key == "Transfer-Encoding")
		{
			if (value.empty())
				throw 400;
			if (value != "chunked")
				throw 501;
		}

		if (key == "Content-Type")
		{
			if (value.empty())
			throw 400;
		}

		headers_map.insert(std::pair<std::string, std::string>(key, value));
	}

	if (this->method == "POST" && (this->headers_map.find("Content-Length") == this->headers_map.end() && this->headers_map.find("Transfer-Encoding") == this->headers_map.end()))
        throw 400;

	if (this->headers_map.find("Host") == this->headers_map.end())
		throw 400;

}

void HttpRequestParser::parse_body(unsigned char *buf, ssize_t nbytes, int &Done)
{
	//handle parsing normal
	if (headers_map.find("Content-Length") != headers_map.end())
	{
		ssize_t bytes_written = write(this->bodyFileFD, buf, nbytes);
		if (bytes_written == -1)
			throw 500;
		ssize_t content_length = atoi(headers_map.find("Content-Length")->second.c_str());
		if (bytes_written > content_length)
			throw 409;
		if (content_length == bytes_written)
		{
			Done = 1;
			close(this->bodyFileFD);
		}
	}
	//handle chunked
	else if (headers_map.find("Transfer-Encoding") != headers_map.end())
	{

	}
}
