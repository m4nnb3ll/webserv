#include "Location.hpp"

// Constructor
Location::Location(Server *server)
	:	_autoIndex(false), _redirectCode(0)
{
	_server = server;
}

Location::Location(const Location &copy)
{
	*this = copy;
}

Location::~Location(){}

Location &Location::operator=(const Location &copy)
{
	if (this != &copy)
	{
		_path = copy._path;
		_rootPath = copy._rootPath;
		_indexes = copy._indexes;
		_autoIndex = copy._autoIndex;
		_allowMethods = copy._allowMethods;
		_cgi = copy._cgi;
		_uploadDir = copy._uploadDir;
	}
	return (*this);
}

std::string		Location::getRootPath() const
{
	return (this->_rootPath);
}

std::string		Location::getUploadDir() const
{
	return (this->_uploadDir);
}

const std::vector<std::string>&	Location::getIndexes() const
{
	return (this->_indexes);
}

const bool&	Location::getAutoIndex() const
{
	return (this->_autoIndex);
}

const std::vector<std::string>&	Location::getAllowMethods() const
{
	return (this->_allowMethods);
}

const std::string&	Location::getRedirectPath() const
{
	return (this->_redirectPath);
}

const bool&	Location::getRedirect() const
{
	return (this->_redirect);
}


const int& Location::getRedirectCode() const
{
	return (this->_redirectCode);
}

void	Location::setPath(std::istringstream &iss)
{
	std::string	path;

	if (!(iss >> path))
		throw std::runtime_error("Invalid ressource type");
	_path = path;
}

const std::vector<std::string>& Location::getServerNames() const
{
	return (this->_server->getServerNames());
}

void	Location::setRootPath(std::istringstream &iss)
{
	std::string	rootPath;
	struct stat	info;
	
	iss >> rootPath;
	if (iss.fail() || !iss.eof())
		throw(std::runtime_error("error in root"));
	if (stat(rootPath.c_str(), &info) != 0)
        throw(std::runtime_error("Error in `root`, make sure that the rootPath does exist!"));
	if (!(info.st_mode & S_IFDIR))
		throw(std::runtime_error("the rootPath is not a directory!"));
	_rootPath = rootPath;
}

void	Location::setUploadDir(std::istringstream &iss)
{
	std::string	uploadDir;
	struct stat	info;
	
	iss >> uploadDir;
	if (iss.fail() || !iss.eof())
		throw(std::runtime_error("error in upload_dir"));
	if (stat(uploadDir.c_str(), &info) != 0)
        throw(std::runtime_error("Error in `upload_dir`, make sure that the upload_dir does exist!"));
	if (!(info.st_mode & S_IFDIR))
		throw(std::runtime_error("the upload_dir is not a directory!"));
	_uploadDir = uploadDir;
}

void	Location::addIndex(std::istringstream &iss)
{
	std::string	index;

	while (iss >> index)
		_indexes.push_back(index);
	if (!iss.eof())
		throw(std::runtime_error("error in index"));
}

void	Location::setAutoIndex(std::istringstream &iss)
{
	std::string	autoIndex;
	
	iss >> autoIndex;
	if (iss.fail() || !iss.eof())
		throw(std::runtime_error("error in autoindex"));
	if (autoIndex == "on")
		_autoIndex = true;
	else if (autoIndex == "off")
		_autoIndex = false;
	else
		throw(std::runtime_error("unrecognized value for autoindex"));
}

const std::string&	Location::getPath() const
{
	return (this->_path);
}

  std::map<std::string, std::string> 	Location::getCgi()  
{
	return (this->_cgi);
}

void	Location::setAllowMethods(std::istringstream &iss)
{
	std::string	method;

	while (iss >> method)
	{
		if (method == "GET" || method == "POST" || method == "DELETE")
			_allowMethods.push_back(method);
		else
			throw(std::runtime_error("unrecognized method: " + method));
	}
	if (!iss.eof())
		throw(std::runtime_error("error in index"));
}

void	Location::setRedirect(std::istringstream &iss)
{
	int			code;
	std::string	path;

	iss >> code >> path;
	if (iss.fail() || !iss.eof())
		throw std::runtime_error("error in redirect");
	if (code != 301 && code != 302)
		throw std::runtime_error("Invalid redirection status code in configuration, use 301 or 302");
	_redirect = true;
	_redirectPath = path;
	_redirectCode = code;
}
