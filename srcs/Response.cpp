#include "Response.hpp"

Response::Response(Request* request)
	:	_request(request), _isFinished(false), _statusCode(STATUS_SUCCESS)
{
	_checkLocation();
	_checkResource();
	_checkCgi();
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

	if (_isFinished) return ;
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
	struct stat	fileStat;
	std::string resourcePath;

	if (_isFinished) return ; // will check this later
	_resource = _request->getLocation()->getRootPath() + _request->_getRequestURI();
	if (stat(_resource.c_str(), &fileStat) != 0)
		_finishWithCode(STATUS_NOT_FOUND);
	else if (S_ISDIR(fileStat.st_mode))
		_resourceType = RT_DIR;
	else
		_resourceType = RT_FILE;
}

// STOPPED HERE
bool	Response::_dirHasIndexFiles(std::vector<std::string> indexes)
{
	DIR				*dir;
	struct dirent	*entry;
	std::string		dirFiles;

	dir = opendir(_resource.c_str());
	if (!dir)
		throw (std::runtime_error("Error opening the dir\n"));
	while (entry = readdir(dir))
		dirFiles.puhs_back(entry->d_name);
	for (size_t i = 0; i < indexes.size(); i++)
		if (std::find(dirFiles.begin(), dirFiles.end(), indexes[i] != dirFiles.end()))
			return (true);
	return (false);
}

void	_runCgi()
{
	// cgi logic will go here
}

bool	Response::_extensionMatch(const std::string& extension, const std::string& filename)
{
	size_t	dotPos = filename.find_last_of('.');

    if (dotPos != std::string::npos)
		return (filename.substr(dotPos) == extension);
    return (false); // No extension found in filename
}

void	Response::_checkCgi()
{
	std::map<std::string, std::string>					cgi;
	std::map<std::string, std::string>::const_iterator	it;
	bool												match = false;

	if (_isFinished) return ;
	cgi = _request->getCgi();
	if (cgi.size())
	{
		if (_resourceType == RT_FILE)
		{
			// _extensionMatch
			for (it = cgi.begin(); (it != cgi.end()) && !match ; ++it)
				match = _extensionMatch(it->first, _resource);
			if (match) _runCgi();
		}
		else if (_dirHasIndexFiles(_request->getLocation()->getIndexes()))
			// STOPPED HERE
	}
}

void	Response::_handleFile()
{
	_checkCgi();
}

void	Response::_handleDir()
{
	std::string	uri;

	if (_request->getMethod() == "DELETE")
	{
		uri = _request->_getRequestURI();
		if (uri[uri.size() - 1] != '/')
			_finishWithCode(STATUS_CONFLICT);
		else
			_checkCgi();
	}
	else
	{
		uri = _request->_getRequestURI();
		if (uri[uri.size() - 1] != '/')
			_finishWithCode(STATUS_MOVED);
		else if (_dirHasIndexFiles(_request->getLocation()->getIndexes()))
			_checkCgi();
		else
			_request->getLocation()->getAutoIndex()
				?	_autoIndexDir()
				:	_finishWithCode(STATUS_FORBIDDEN);
	}
}

void	Response::_handleGet()
{
	_checkResource();
}

void	Response::_handlePost()
{
	if (_canUpload())
		_uploadFile();
	else
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