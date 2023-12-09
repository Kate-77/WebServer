# include "Response.hpp"

Response::Response()
{
    return ;
}

Response::~Response()
{
    return ;
}

Response::Response(Response const & src)
{
    *this = src;
    return ;
}

void    Response::setResponse(Request & request, WebServ & server)
{
    this->setBody(request, server);
    this->setStatus(request, server);
    this->setLocation(request, server);
    this->setContentType(request, server);
    this->setContentLength(request, server);
    this->setServer(request, server);
    this->setError(request, server);
    this->setErrorStatus(request, server);
    this->_response = this->_status + this->_location + this->_content_type + this->_content_length + this->_server + this->_error + this->_error_status + "\r\n" + this->_body;
    return ;
}