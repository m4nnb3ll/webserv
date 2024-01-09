/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:31:55 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/09 12:23:15 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request* request)
	:	_request(request), _isFinished(false), _statusCode(STATUS_SUCCESS)
{
	_checkLocation();
	_checkResource();
	_checkCgi();
	_checkMethod();
	_errorCheck();
	std::cout << "The status code is: " << YELLOW << _statusCode << RESET_COLOR << std::endl;
}

bool	Response::isFinished() const
{
	return (_isFinished);
}

std::string	Response::getContent() const
{
	return (_content);
}

void	Response::_handleGet()
{
	if (_isFinished)
		return ;
	if (_resourceType == RT_FILE)
	{
		// std::cout << "the request file is: " << _resource << std::endl;
		_returnFile(_resource);
	}
	else
	{
		_checkDirURI();
		if (!_isFinished)
		{
			_dirHasIndexFiles(_request->getLocation()->getIndexes())
				?	_returnFile(_resource + _index)
				:	_checkAutoIndex();
		}
	}
}

void	Response::_handlePost()
{
	if (_isFinished) return ;
	_request->getLocation()->getUploadDir().empty()
		?	_finishWithCode(STATUS_FORBIDDEN)
		:	_uploadFile();
}

void	Response::_handleDelete()
{
	if (_isFinished) return ;
	if (_resourceType == RT_FILE)
		_deleteFile();
	else
	{
		_checkDirURI();
		if (!_isFinished)
			_tryDeleteDir();
	}
}


std::string	Response::_getStatusCodeMsg()
{
	switch (_statusCode)
	{
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

std::string	Response::_getErrFilePath()
{
	std::map<int, const std::string>	errorPages;
	std::string							filePath;

	errorPages = _request->getLocation()->getServer()->getErrorPages();
	filePath = errorPages[_statusCode];
	if (!filePath.empty())
		return (filePath);
	else
	{
		std::ostringstream	oSS;

		oSS << DEFAULT_ERR_DIR << _statusCode << ".html";
		return (oSS.str());
	}
}

void	Response::_errorCheck()
{
	if (_statusCode >= 400 && _statusCode < 600)
	{
		std::string			errFilePath = _getErrFilePath();
		std::ifstream		file(errFilePath.c_str(), std::ios::binary);
		std::ostringstream	finalStream;
		std::ostringstream	fileContent;

		fileContent << file.rdbuf(); // Read the file content into a stringstream
		// Print HTTP response headers
		finalStream << "HTTP/1.1" << " " << _statusCode << " " << _getStatusCodeMsg() << "\r\n";
		finalStream << "Content-Length: " << fileContent.str().length() << "\r\n";
		finalStream << "Content-Type: text/html\r\n";
		finalStream << "\r\n"; // Empty line to separate headers from content
		// Print file content
		finalStream << fileContent.str();
		file.close();
		_content = finalStream.str();
	}
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
	if (_statusCode == STATUS_CREATED
		|| _statusCode == STATUS_NO_CONTENT || _statusCode == STATUS_MOVED)
	{
		std::ostringstream	oSS;
		oSS << "HTTP/1.1" << " " << _statusCode << " " << _getStatusCodeMsg() << "\r\n";
		if (_statusCode == STATUS_MOVED)
			oSS << "Location: " << _resource + "/" << "\r\n";
		oSS << "Content-Length: 0\r\n";
		oSS << "\r\n";
		_content = oSS.str();
	}
	_isFinished = true;
}

void	Response::_redirect()
{
	std::ostringstream	oSS;

	_statusCode = STATUS_MOVED;
	oSS << "HTTP/1.1" << " " << _statusCode << " " << _getStatusCodeMsg() << "\r\n";
	oSS << "Location: " << _request->getLocation()->getRedirectPath() << "\r\n";
	oSS << "Content-Length: 0\r\n";
	oSS << "\r\n";
	_content = oSS.str();
	_isFinished = true;
}

void	Response::_checkLocation()
{
	Location	*location;

	if (_isFinished) return ;
	location = _request->getLocation();
	if (!location)
	{
		std::cout << "couldn't find the location" << std::endl;
		_finishWithCode(STATUS_NOT_FOUND);
	}
	else if (location -> getRedirect())
		_redirect();
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

	if (_isFinished) return ;
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
	_resource = _request->getLocation()->getRootPath() + _request->getUri();
	if (stat(_resource.c_str(), &fileStat) != 0)
	{
		std::cout << "couldn't find the resource" << " " << _resource << std::endl;
		_finishWithCode(STATUS_NOT_FOUND);
	}
	else if (S_ISDIR(fileStat.st_mode))
		_resourceType = RT_DIR;
	else
		_resourceType = RT_FILE;
}

bool	Response::_dirHasIndexFiles(std::vector<std::string> indexes)
{
	DIR											*dir;
	struct dirent								*entry;
	std::vector<std::string>					dirFiles;
	std::vector<std::string>::const_iterator	indexIt;

	dir = opendir(_resource.c_str());
	if (!dir)
		throw (std::runtime_error("Error opening the dir\n"));
	while ((entry = readdir(dir)))
		dirFiles.push_back(entry->d_name);
	for (size_t i = 0; i < indexes.size(); i++)
	{
		if ((indexIt = std::find(dirFiles.begin(), dirFiles.end(), indexes[i]))
			!= dirFiles.end())
		{
			_index = *indexIt;
			return (true);
		}
	}
	return (false);
}

void	Response::_runCgi()
{
	// cgi logic will go here
	std::cout << BLUE << "CGI RUNNING..." << RESET_COLOR << std::endl;
	_statusCode = STATUS_SUCCESS;
	_isFinished = true;
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
	std::vector<std::string>							indexes;

	if (_isFinished) return ;
	cgi = _request->getLocation()->getCgi();
	if (cgi.size())
	{
		if (_resourceType == RT_FILE)
		{
			// _extensionMatch
			for (it = cgi.begin(); (it != cgi.end()) && !match ; ++it)
				match = _extensionMatch(it->first, _resource);
			if (match) _runCgi();
		}
		else
		{
			if (_dirHasIndexFiles(_request->getLocation()->getIndexes()))
			{
				// thinking about placing the following logic in a function
				// or place the extension check inside the _runCgi function
				for (it = cgi.begin(); (it != cgi.end()) && !match ; ++it)
					match = _extensionMatch(it->first, _resource + _index);
				if (match) _runCgi();
			}

		}
	}
}

void	Response::_checkDirURI()
{
	std::string	uri = _request->getUri();

	if (uri[uri.size() - 1] != '/')
		_request->getMethod() == "DELETE"
			?	_finishWithCode(STATUS_CONFLICT)
			:	_finishWithCode(STATUS_MOVED);
}

std::string	Response::_strToLower(const std::string& input) {
    std::string result = input;

    for (size_t i = 0; i < result.length(); ++i)
        result[i] = static_cast<char>(std::tolower(result[i]));
    return result;
}

std::string	Response::_getMimeType(const std::string& filename) 
{
    // Extract file extension
    size_t	dotIndex = filename.find_last_of(".");
    if (dotIndex == std::string::npos || dotIndex == filename.length() - 1)
        return "application/octet-stream";

    std::string extension = _strToLower(filename.substr(dotIndex));
    // Look up the extension in the global map
    std::map<std::string, std::string>::iterator it = g_mimeTypes.find(extension);
    return (it != g_mimeTypes.end() ? it->second : "application/octet-stream");
}

void	Response::_returnFile(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
    
	if (file)
	{
		std::ostringstream	finalStream;
		std::ostringstream	fileContent;
		fileContent << file.rdbuf(); // Read the file content into a stringstream
		// Print HTTP response headers
		finalStream << "HTTP/1.1 200 OK\r\n";
		finalStream << "Content-Length: " << fileContent.str().length() << "\r\n";
		finalStream << "Content-Type: "<< _getMimeType(filename) << "\r\n";
		finalStream << "\r\n"; // Empty line to separate headers from content
		// Print file content
		finalStream << fileContent.str();
		file.close();
		_content = finalStream.str();
		_finishWithCode(STATUS_SUCCESS);
	}
	else
		_finishWithCode(STATUS_INTERNAL_ERR);
}

void	Response::_returnDirAutoIndex()
{
    DIR*			dir;
    struct dirent*	entry;

    dir = opendir(_resource.c_str());
    if (dir != NULL)
	{
		std::ostringstream	finalStream;
		std::ostringstream	html;

        html << "<html><head><title>Directory Index</title></head><body><h1>Index of The Directory</h1><ul>";
        while ((entry = readdir(dir)) != NULL)
            html << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
		html << "</ul></body></html>";
		closedir(dir);

		finalStream << "HTTP/1.1 200 OK\r\n";
		finalStream << "Content-Length: " << html.str().length() << "\r\n";
		finalStream << "Content-Type: text/html\r\n";
		finalStream << "\r\n";
		finalStream << html.str();
		_content = finalStream.str();
		_finishWithCode(STATUS_SUCCESS);
    }
	else
		_finishWithCode(STATUS_INTERNAL_ERR);
}

void	Response::_checkAutoIndex()
{
	if (_request->getLocation()->getAutoIndex())
		_returnDirAutoIndex();
	else
		_finishWithCode(STATUS_FORBIDDEN);
}

void	Response::_uploadFile()
{
	if (!_request->files.size())
	{
		_finishWithCode(STATUS_INTERNAL_ERR);
		return ;
	}

	std::string	filename = "uploads/" + _request->files[0].first;
	std::ofstream file(filename);

	if (file.is_open())
	{
		file << _request->files[0].second;
		file.close();
		_finishWithCode(STATUS_CREATED);
	}
	else
		_finishWithCode(STATUS_INTERNAL_ERR);
}

void	Response::_deleteFile()
{
	int	deletionRes;

	deletionRes = std::remove(_resource.c_str());
	if (deletionRes == 0)
		_finishWithCode(STATUS_NO_CONTENT);
	else
		_finishWithCode(STATUS_INTERNAL_ERR);
}

void	Response::_tryDeleteDir()
{
	if (access(_resource.c_str(), W_OK) != 0)
		_finishWithCode(STATUS_FORBIDDEN);
	else
	{
		std::string	command = "rm -rf " + _resource;
		int result = std::system(command.c_str());
		(result == 0)
			?	_finishWithCode(STATUS_NO_CONTENT)
			:	_finishWithCode(STATUS_INTERNAL_ERR);
	}
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