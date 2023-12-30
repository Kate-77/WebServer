#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {
	status_code_ = -1;
}

HttpRequestParser::~HttpRequestParser() {}

HttpRequestParser::HttpRequestParser(const HttpRequestParser &http_request_parser) {
	*this = http_request_parser;
}

HttpRequestParser &HttpRequestParser::operator=(const HttpRequestParser &http_request_parser) {
	this->status_code_ = http_request_parser.status_code_;
	return *this;
}

int HttpRequestParser::getStatusCode() const {
	return status_code_;
}

void HttpRequestParser::setStatusCode(int status_code) {
	status_code_ = status_code;
}



void HttpRequestParser::parseRequest(ssize_t nbytes, unsigned char *buf, int &Done)
{
	buf[nbytes] = '\0';
	Done = 1;
}


