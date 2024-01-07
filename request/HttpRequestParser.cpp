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
	bodyFileName = "./cache/" + generateName();
	bodyFileFD = -1;
	req_done = false;
	reqLine_Headers = "";
	bytes_written = 0;
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
	this->req_done = http_request_parser.req_done;
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

bool HttpRequestParser::getReqDone() const
{
	return req_done;
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

void HttpRequestParser::setReqDone(bool req_done)
{
	this->req_done = req_done;
}

void HttpRequestParser::parseRequest(ssize_t nbytes, unsigned char *buf, int &Done)
{
	unsigned char* bodyStart = buf;

	std::string reqLine_Headers;
	size_t reqLine_Headers_Length;
	size_t bodyLength = 0;
	bool body_first = false;

	// buf[nbytes] = '\0';

	if (req_done == false)
	{
		// //printf("header parse\n");
		const char* doubleCRLF = "\r\n\r\n";
		unsigned char* bodyStartPtr = static_cast<unsigned char*>(memmem(buf, nbytes, doubleCRLF, strlen(doubleCRLF)));
		
		if (bodyStartPtr != NULL)
		{
			reqLine_Headers_Length = bodyStartPtr - buf + 2;
			bodyStart = bodyStartPtr + strlen(doubleCRLF);
			buf[reqLine_Headers_Length] = '\0';
			reqLine_Headers += reinterpret_cast<char *>(buf);
		}
		else
		{
			reqLine_Headers += reinterpret_cast<char *>(buf);
			return;
		}
   		parseRequestLine_Headers(reqLine_Headers);
		// //printf("method **********= [%s]\n", getMethod().c_str());
		// //printf("path = [%s]\n", getPath().c_str());
		// //printf("headers done\n");
		if (bodyStart)
			body_first = true;
		req_done = true;
	}

	if (getMethod() ==  "POST" &&  bodyStart && req_done)
	{
		// //printf("body parse\n");
		// //printf("body start = [%s]\n", bodyStart);
		bodyLength = nbytes;
		if (body_first)
		{
        	bodyLength = nbytes - reqLine_Headers_Length - 2;
		}
		parse_body(bodyStart, bodyLength, Done);
	}
	else if (req_done && getMethod() != "POST")
		Done = 1;
}

void HttpRequestParser::parseRequestLine_Headers(std::string reqLine_Headers)
{
	std::istringstream iss(reqLine_Headers);
	std::string request_line;

	////printf("reqLine_Headers = [%s]\n", reqLine_Headers.c_str());
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

void HttpRequestParser::parse_body(unsigned char *buf, size_t nbytes, int &Done)
{
	//handle parsing normal
	if (headers_map.find("Content-Length") != headers_map.end())
	{
		size_t content_length = std::stoi(headers_map.find("Content-Length")->second.c_str());
		// //printf("nbytes		   = [%ld]\n", nbytes);
		if (write(this->bodyFileFD, buf, nbytes) != -1)
			this->bytes_written += nbytes;
		else
			throw 500;
		if (content_length == bytes_written)
		{
			Done = 1;
			close(this->bodyFileFD);
		}
		else if (bytes_written > content_length)
		{
		// //printf("bytes_written  = [%ld]\n", bytes_written);
		// //printf("content_length = [%ld]\n", content_length);
			throw 409;
		}
	}
	//handle chunked
	else if (headers_map.find("Transfer-Encoding") != headers_map.end())
	{
		//printf("chunked*****\n");
		parse_chunked_body(buf, nbytes, Done);
	}
	else
	{
		std::cerr << "___THROW__411___" << std::endl;
		throw 411;
	}
}
bool verif_chunk(unsigned char *buffer)
{
    std::string firstLine(reinterpret_cast<char *>(buffer));
    std::size_t pos = firstLine.find("\r\n");
    std::string hexNum = firstLine.substr(0, pos);

    if (pos != std::string::npos)
    {
        for (std::string::iterator it = hexNum.begin(); it != hexNum.end(); ++it)
        {
            if (!std::isxdigit(*it))
                return false;
        }
        return true;
    }
    return false;
}

void HttpRequestParser::parse_chunked_body(unsigned char *buf, ssize_t nbytes, int &Done)
{
	if (memmem(buf, nbytes, "0\r\n\r\n", 5) == NULL)
	{
		if (memmem(buf, nbytes, "\r\n", 2) != NULL && verif_chunk((unsigned char *)memmem(buf, nbytes, "\r\n", 2) + 2))
		{
				ssize_t lefts = (unsigned char *)memmem(buf, nbytes, "\r\n", 2) - buf;
				write(this->bodyFileFD, buf, lefts);
				ssize_t chunk = nbytes - lefts - 2;
				buf = (unsigned char *)memmem(buf, nbytes, "\r\n", 2) + 2;
				chunk = chunk - ((unsigned char *)memmem(buf, chunk, "\r\n", 2) + 2 - buf);
				buf = (unsigned char *)memmem(buf, chunk, "\r\n", 2) + 2;
				write(this->bodyFileFD, buf, chunk);

				// unsigned char *tmp = (unsigned char *)memmem(buf, chunk, "\r\n", 2) + 2;
				// chunk = chunk - (tmp - buf);
				// if (chunk >0 )
				// {
				// // 	// buf = tmp;
				// // 	// parse_chunked_body(buf, chunk, Done);
				// // 	// //printf("tmp = [%s]\n", tmp);
				// write(this->bodyFileFD, "*****", 5);
				// write(this->bodyFileFD, tmp, chunk);
				// 	throw 999;
				// }
			// }
		}
		else
		{
			if(verif_chunk(buf))
			{
				nbytes = nbytes - ((unsigned char *)memmem(buf, nbytes, "\r\n", 2) + 2 - buf);
				buf = (unsigned char *)memmem(buf, nbytes, "\r\n", 2) + 2;
			}
			if (write(this->bodyFileFD, buf, nbytes) == -1)
				throw 500;
		}
	}
	else
	{
		
		nbytes = nbytes - 7;
				// //printf("nbytes = [%ld]\n", nbytes);
				// //printf("buf = [%s]\n", buf);
		// if (nbytes > 0)
		// {
			if(write(this->bodyFileFD, buf, nbytes) == -1)
				// {
				// 	//printf("buf = [%s]\n", buf);
				// 	//printf("nbytes = [%ld]\n", nbytes);
				throw 500;
				// }
				// else 
				// //printf("mzyaaaaaaaan\n");

		// }
		Done = 1;
	}
}

