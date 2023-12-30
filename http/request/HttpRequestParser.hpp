#pragma once
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <map>

class HttpRequestParser {
	private:
		int status_code_;
	public:

		//canonical form
		HttpRequestParser();
		~HttpRequestParser();
		HttpRequestParser(const HttpRequestParser &http_request_parser);
		HttpRequestParser& operator=(const HttpRequestParser &http_request_parser);


		//getters
		int getStatusCode() const;

		//setters
		void setStatusCode(int status_code);
		

		void parseRequest(ssize_t nbytes, unsigned char *buf, int &Done);
};