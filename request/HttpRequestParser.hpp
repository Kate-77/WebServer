#pragma once
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <map>
#include <fcntl.h>

class HttpRequestParser {
	private:
		int status_code_;
		std::string method;
		std::string path;
		std::string query;
		std::string version;
		std::string bodyFileName;
		int bodyFileFD;

		std::map<std::string, std::string> headers_map;//
	public:

		//canonical form
		HttpRequestParser();
		~HttpRequestParser();
		HttpRequestParser(const HttpRequestParser &http_request_parser);
		HttpRequestParser& operator=(const HttpRequestParser &http_request_parser);


		//getters
		int getStatusCode() const;
		std::string getMethod() const;
		std::string getPath() const;
		std::string getQuery() const;
		std::string getVersion() const;
		std::string getBodyFileName() const;
		int getBodyFileFD() const;

		//setters
		void setStatusCode(int status_code);
		void setMethod(std::string method);
		void setPath(std::string path);
		void setQuery(std::string query);
		void setVersion(std::string version);
		void setBodyFileName(std::string bodyFileName);
		void setBodyFileFD(int bodyFileFD);
		

		void parseRequest(ssize_t nbytes, unsigned char *buf, int &Done);
		void parseRequestLine_Headers(std::string reqLine_Headers);
		// void parseBody(unsigned char* bodyStart, size_t bodyLength);
		void parse_request_line(std::string request_line);
		std::string generateName();
		void parse_headers(std::istringstream &headers);
};