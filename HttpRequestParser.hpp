#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <map>

class HttpRequestParser {
	private:
		std::string method_;
		std::string uri_;
		std::string version_;
		std::map<std::string, std::string> headers_;
		std::string body_;
		
		void parseRequestLine(const std::string& requestLine);
		void parseHeaderLine(const std::string& headerLine);
	public:
		void parseRequest(const std::string& request);
		void printParsedInfo() const;

};