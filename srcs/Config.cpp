/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:31:37 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/10 15:53:25 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::map<std::string, std::string>	g_mimeTypes;

void	initMimeTypes()
{
    std::ifstream file(MIMETYPES_FILE_PATH);
    if (!file.is_open())
		throw(std::runtime_error("error opening the MIMETYPES file"));

    std::string line;
    while (std::getline(file, line))
	{
        std::istringstream linestream(line);
        std::string extension;
        std::string mimeType;
		std::getline(linestream, extension, ',');
		std::getline(linestream, mimeType);
		g_mimeTypes[extension] = mimeType;
    }
    file.close();
	std::cout << RED << "initialized mime types" << RESET_COLOR << std::endl;
}

// there is a problem with the freeing 
Config::~Config()
{
	std::set<ServersSocket*>	ptrs;
	for (std::map<int, ServersSocket*>::iterator it = _sdToServersSocket.begin();
		it != _sdToServersSocket.end(); it++)
	{
		close(it->first);
		ptrs.insert(it->second);
	}
	for (std::set<ServersSocket*>::iterator it = ptrs.begin(); it != ptrs.end(); it++)
		delete *it;
    for (std::map<int, Client*>::iterator it = _sdToClient.begin(); it != _sdToClient.end(); ++it)
		delete (it->second);
}

Config::Config(const std::string &input)
{
	std::string	line;

	initMimeTypes();
	_openConfig(input);
	while (std::getline(_configFile, line))
	{
		_ignoreComments(line);
		if (line.empty()) continue ;
		std::istringstream	iss(line);
		if ( (iss >> line) && (line == "server") && iss.eof())
			_parseServer();
		else
			throw std::runtime_error("Unknown type in configuration file: " + line);
	}
	_initSockets();
}

std::string&	Config::_ignoreComments(std::string& line)
{
	std::istringstream iss(line);
	std::getline(iss, line, ';');
	iss.clear();
	iss.str(line);
	std::getline(iss, line, '#');
	return (line);
}

void Config::_openConfig(const std::string &input)
{
	_configFile.open(input.c_str());
	if (!_configFile.is_open())
		throw std::runtime_error("Failed to open configuration file");
}

std::istream&	Config::_getEntry(std::ifstream& ifs, std::string& line)
{
	while ((std::getline(ifs, line)) && _ignoreComments(line).empty());
	return (ifs);
}

unsigned long	ft_inet_addr(const std::string& ipAddr)
{
    std::string					octet;
    int							octets[4];
	std::string::size_type		pos[2] = {0};// index 0 for begining, 1 for end
	unsigned long				result = 0;

   for (int i = 0; i < 4; i++)
	{
		pos[1] = ipAddr.substr(pos[0]).find('.');
		if (pos[1] == std::string::npos) pos[1] = ipAddr.size();
		octets[i] = std::atoi(ipAddr.substr(pos[0], pos[1]).c_str());
		pos[0] += pos[1] + 1;
	}
    for (size_t i = 0; i < 4; ++i)
        result |= (static_cast<unsigned long>(octets[i]) << ((3 - i) * 8));
    return (htonl(result));
}

bool isValidIPv4(const std::string& ip) {
    std::istringstream iss(ip);
    int num;
    char dot;

    for (int i = 0; i < 4; ++i)
        if (!(iss >> num)
			|| (i < 3 && !(iss >> dot) && dot != '.') || (num < 0 || num > 255))
            return false;
    return true;
}

std::string	Config::_extractListen(std::istringstream& iss)
{
	std::string			ip;
	int					portNum;
	std::ostringstream	oss;

	if (iss.str().find(':') != std::string::npos)
	{
		std::getline(iss, ip, ':');
		if (!isValidIPv4(ip))
			throw std::runtime_error("invalid ip address!");
	}
	if (iss >> portNum)
	{
		if (iss.fail() || !iss.eof())
			throw(std::runtime_error("error in listen directive"));
	}
	else
		throw std::runtime_error("error in listen directive");
	oss << ip << ':' << portNum;
	return (oss.str());
}

bool	Config::_portExists(const std::string& s)
{
	if (_portToServersSocket.find(s) != _portToServersSocket.end())
		return (true);
	return (false);
}

void Config::_parseListen(std::istringstream& iss, Server* srv)
{
	const std::string&	port(_extractListen(iss));

	if (_portExists(port))
		_portToServersSocket[port]->addServer(srv);
	else
		_portToServersSocket.insert(std::make_pair(port, new ServersSocket(srv)));
}

void Config::_parseServerNames(std::istringstream &iss, Server* srv)
{
	std::string serverName;

	while (iss >> serverName)
		srv->addServerName(serverName);
	if (!iss.eof())
		throw(std::runtime_error("error in server_names"));
}

// I ll need to check error_pages presence later
void	_parseErrorPages(std::istringstream &iss, Server* srv)
{
	std::string	errorPage;
	int			errorCode;

	iss >> errorCode >> errorPage;
	if (iss.fail() || !iss.eof())
		throw(std::runtime_error("error in error_page"));
	srv->setErrorPages(errorCode, errorPage);
}

void	_parseClientMaxBodySize(std::istringstream &iss, Server* srv)
{
	size_t	maxBodySize;

	iss >> maxBodySize;
	if (iss.fail() || !iss.eof())
		throw std::runtime_error("error in max body size");
	srv->setMaxBodySize(maxBodySize);
}

void Location::addCgi(std::istringstream& iss)
{
	std::string	cgi_ext, cgi_path;

	iss >> cgi_ext >> cgi_path;
	if (iss.fail() || !iss.eof())
		throw(std::runtime_error("error in cgi"));
	_cgi.insert(std::make_pair(cgi_ext, cgi_path));
}

void Config::_parseLocation(std::istringstream& iss, Server* srv)
{
	Location	*location;
	std::string line;
	
	location = new Location(srv);
	srv->addLocation(location);
	location->setPath(iss);
	if ( !(iss >> line) || (line != "{") || !iss.eof())
		throw std::runtime_error("Invalid location block");
	while (_getEntry(_configFile, line))
	{
		iss.clear();
		iss.str(line);
		iss >> line;
		if (line == "location")
			throw std::runtime_error("Location block inside location block");
		if (line == "root")
			location->setRootPath(iss);
		else if (line == "upload_dir")
			location->setUploadDir(iss);
		else if (line == "index")
			location->addIndex(iss);
		else if (line == "autoindex")
			location->setAutoIndex(iss);
		else if (line == "allow_methods")
			location->setAllowMethods(iss);
		else if (line == "return")
			location->setRedirect(iss);
		else if (line == "cgi")
			location->addCgi(iss);
		else if (line == "}")
			break ;
		else
			throw std::runtime_error("Unknown type in configuration file");
	}
}

void Config::_parseServer()
{
	std::string	line;
	Server		*srv;

	srv = new Server();
	if (!_getEntry(_configFile, line))
		throw std::runtime_error("Invalid server block");
	std::istringstream	iss(line);
	if ( !(iss >> line) || (line != "{") || !iss.eof())
		throw std::runtime_error("Invalid server block");
	while (_getEntry(_configFile, line))
	{
		iss.clear();
		iss.str(line);
		iss >> line;
		if (line == "server")
			throw std::runtime_error("server inside a server block!");
		if (line == "listen")
			_parseListen(iss, srv);
		else if (line == "server_name")
			_parseServerNames(iss, srv);
		else if (line == "error_page")
			_parseErrorPages(iss, srv);
		else if (line == "client_max_body_size")
			_parseClientMaxBodySize(iss, srv);
		else if (line == "location")
			_parseLocation(iss, srv);
		else if (line == "}")
			break ;
		else
			throw std::runtime_error("Unknown type in configuration file: " + line);
	}
	if (line != "}")
		throw std::runtime_error("Unclosed bracket");
}

void Config::_unplugSocket(ServersSocket *sS)
{
	std::cout << "The socket about to unplug is: " << sS << std::endl;
	for (std::map<std::string, ServersSocket*>::iterator it = _portToServersSocket.begin();
		it != _portToServersSocket.end(); it++)
	{
		if (it->second == sS)
		{
			_portToServersSocket.erase(it->first);
			delete sS;
			sS = NULL;
			break ;
		}
	}
}

struct sockaddr_in	Config::strToSockaddrin(const std::string& port)
{
	std::istringstream			iss(port);
	std::string					ip;
	int							portNum;
	struct sockaddr_in			addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	if (iss.str().find(':') != std::string::npos)
	{
		std::getline(iss, ip, ':');
		addr.sin_addr.s_addr = ft_inet_addr(ip);
	}
	if (iss >> portNum)
		addr.sin_port = htons(portNum);
	else
		throw std::runtime_error("error in listen directive");
	return (addr);
}

void	Config::_addPollfd(int sd, short events)
{
	struct pollfd	newPollFd;

	newPollFd.fd = sd;
	newPollFd.events = events;
	_pollFds.push_back(newPollFd);
}

void	Config::_rmPollfd(int sd)
{
	/*test*/
	// std::cout << YELLOW << "I GET INTO THE REMOVE" << RESET_COLOR << std::endl;
	// delete _sdToServersSocket[sd];
	_sdToServersSocket.erase(sd);
	/*test*/
	// remove from pollfds
	for (std::vector<struct pollfd>::iterator it = _pollFds.begin();
		it != _pollFds.end(); it++)
	{
		if (it -> fd == sd)
		{
			_pollFds.erase(it);
			break ;
		}
	}
	if (close(sd) == 0) std::cout << "sd closed" << std::endl;
}

void	Config::_initSockets()
{
	for (std::map<std::string, ServersSocket*>::iterator it = _portToServersSocket.begin();
		it != _portToServersSocket.end(); it++)
	{
		// Create a socket (IPv4, TCP)
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1)
			throw std::runtime_error("Failed to create socket ");
		int optval = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		{
			close(sockfd);
			throw std::runtime_error("Failed to set socket options");
		}
		struct sockaddr_in	sockaddrin = strToSockaddrin(it->first);
		if (bind(sockfd, (struct sockaddr *)&sockaddrin, sizeof(sockaddrin)) < 0)
		{
			perror("error: ");
			throw std::runtime_error("Failed to bind to port.");
		}
		// 25 seems a good back log to work with siege
		if (listen(sockfd, 25) < 0)
			throw std::runtime_error("Failed to listen on socket.");
		it->second->setFd(sockfd);
		_sdToServersSocket.insert(std::make_pair(sockfd, it->second));
		_addPollfd(sockfd, POLLIN);
	}
}

std::map<int, Client*>	Config::getSdToClient() const
{
	return (this->_sdToClient);
}

void	Config::insertToSdToClient(std::pair<int, Client*> pair)
{
	// std::cout << RED << "INSERTION!!!" << RESET_COLOR << std::endl;
	_sdToClient.insert(pair);
}

ServersSocket*	Config::getServersSocket(int sd) const
{
	std::map<int, ServersSocket*>
		::const_iterator	it = _sdToServersSocket.find(sd);

	if (it != _sdToServersSocket.end())
		return (it->second);
    return (NULL);
}
