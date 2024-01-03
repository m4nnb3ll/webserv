#include "Response.hpp"

Response::Response(Request* request)
	:	_request(request), _isFinished(false), _statusCode(STATUS_SUCCESS)
{
	_checkLocation();
	_checkMethod();
	std::cout << "The status code is: " << YELLOW << _statusCode << RESET_COLOR << std::endl;
}

// bool	Response::isError()
// {
// 	if (_statusCode >= 400 && _statusCode < 600)
// 		return (true);
// 	return (false);
// }

void	Response::_finishWithCode(enum e_status_code code)
{
	_statusCode = code;
	_isFinished = true;
}

std::string	Response::_genStatusMsg(int code)
{
	switch (code)
	{
		case 200:
			return ("OK");
		case 201:
			return ("Created");
		case 204:
			return ("No Content");
		case 301:
			return ("Moved Permanently");
		case 400:
			return ("Bad Request");
		case 403:
			return ("Forbidden");
		case 404:
			return ("Not Found");
		case 405:
			return ("Method Not Allowed");
		case 409:
			return ("Conflict");
		case 413:
			return ("Content Too Large");
		case 414:
			return ("URI Too Long");
		case 500:
			return ("Internal Server Error");
		default :
			return ("Not Implemented");

	}
}

void	Response::_checkLocation()
{
	Location	*location;

	location = _request->getLocation();
	if (!location)
		_finishWithCode(STATUS_NOT_FOUND);
	else if (location -> getRedirect())
		_finishWithCode(STATUS_MOVED);
	else
	{
		const std::vector<std::string>&	allowMethods = location->getAllowMethods();
		if (std::find(allowMethods.begin(), allowMethods.end(), _request->getMethod())
			== allowMethods.end())
			_finishWithCode(STATUS_NOT_ALLOWED);
	}
}

void	Response::_checkMethod()
{
	std::string	method = _request->getMethod();

	if (method == "GET")
		_handleGet();
	else if (method == "POST")
		_handlePost();
	else
		_handleDelete();
}

void	Response::_checkResource()
{
	struct stat	buffer;
	std::string resourcePath;

	if (_isFinished) return ;
	resourcePath = _request->getLocation()->getRootPath() + _request->_getRequestURI();
	if (stat(resourcePath.c_str(), &buffer) != 0)
		_finishWithCode(STATUS_NOT_FOUND);
}

void	Response::_handleGet()
{
	_checkResource();
}

void	Response::_handlePost()
{
	_checkResource();
}

void	Response::_handleDelete()
{
	_checkResource();
}

// Leave to later
// void	Response::_checkFinalMsg()
// {
// 	int					statusCode;
// 	std::ostringstream	oSS;

// 	statusCode = _response.getStatusCode();
// 	oSS << "HTTP/1.1" << " " << statusCode << " " << _response.genStatusMsg(statusCode) << "\r\n";
// 	if (_response.getContent().empty())
// 	{
// 		_reponse.checkErrorContent();
// 		oSS << _response.spreadErrorFile

// 	}
// }