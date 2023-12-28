#include "Client.hpp"

Client::Client() {
	this->client_socket = -1;
	memset(&this->client_address, 0, sizeof(this->client_address));
}

Client::~Client() {
}

Client::Client(const Client &client) {
	this->client_socket = client.client_socket;
	this->client_address = client.client_address;
}

Client &Client::operator=(const Client &client) {
	this->client_socket = client.client_socket;
	this->client_address = client.client_address;
	return *this;
}

int Client::getClientSocket() {
	return this->client_socket;
}

sockaddr_in Client::getClientAddress() {
	return this->client_address;
}

void Client::setClientSocket(int client_socket) {
	this->client_socket = client_socket;
}

void Client::setClientAddress(sockaddr_in client_address) {
	this->client_address = client_address;
}
