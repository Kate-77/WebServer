#include "HttpRequestParser.hpp"

void HttpRequestParser::parseRequest(const std::string &request)
{
	std::istringstream stream(request);
    std::string requestLine;
    std::getline(stream, requestLine);

    parseRequestLine(requestLine);

    std::string headerLine;
    while (std::getline(stream, headerLine) && !headerLine.empty()) {
        parseHeaderLine(headerLine);
    }

    // Read the body if present
    std::getline(stream, body_);
}

void HttpRequestParser::printParsedInfo() const
{
	std::cout << "Method: " << method_ << std::endl;
    std::cout << "URI: " << uri_ << std::endl;
    std::cout << "Version: " << version_ << std::endl;

    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers_.begin(); it != headers_.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Body: " << body_ << std::endl;
}

void HttpRequestParser::parseRequestLine(const std::string& requestLine) {
    std::istringstream lineStream(requestLine);
    lineStream >> method_ >> uri_ >> version_;
}

void HttpRequestParser::parseHeaderLine(const std::string& headerLine) {
    size_t colonPos = headerLine.find(':');
    if (colonPos != std::string::npos) {
        std::string key = headerLine.substr(0, colonPos);
        std::string value = headerLine.substr(colonPos + 2); // Skip ': ' after the key
        headers_[key] = value;
    }
}

