# HTTP Server in C++

This is a simple HTTP server implemented in C++, capable of handling GET, POST, and DELETE requests. The server supports multiplexing to handle multiple connections concurrently.

## Features

- **GET Requests:** Retrieve information or resources from the server.
- **POST Requests:** Submit data to be processed to a specified resource.
- **DELETE Requests:** Delete a specified resource on the server.
- **Multiplexing:** Concurrently handle multiple client connections.

## Team

This project was developed by:

- [Kaoutar Moutaouakil](https://github.com/Kate-77)
- [Fatima Ezzahra El Fil](https://github.com/zowaa)
- [Abdelkhalek Boutahri](https://github.com/abboutah)

## Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/Kate-77/WebServer.git
   ```

2. Build the server:
   ```bash
   make 
   ```

3. Run the server with default config file:
   ```bash
   ./webserv 
   ```

4. Run the server with a specific config file:
   ```bash
   ./webserv config_files/cgi_test.conf
   ```

5. Access the server in your web browser or use tools like curl and postman to send requests.