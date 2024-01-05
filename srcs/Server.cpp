#include "Server.hpp"

Server::Server()
	:	_maxBodySize(0)
{}

Server::~Server()
{
	// delete Locations
	for (std::vector<Location*>::iterator it = _locations.begin();
	it != _locations.end(); it++)
		delete *it;
}

void	Server::addServerName(const std::string& serverName)
{
	_serverNames.push_back(serverName);
}

void	Server::setErrorPages(int errCode, const std::string& errPage)
{
	_errorPages.insert(std::make_pair(errCode, errPage));
}

void	Server::setMaxBodySize(size_t maxBodySize)
{
	_maxBodySize = maxBodySize;
}

void Server::addLocation(Location *location)
{
	_locations.push_back(location);
}

void	print_addr(struct sockaddr_in addr);

const std::vector<std::string>&	Server::getServerNames() const
{
	return (_serverNames);
}

const std::vector<Location*>&	Server::getLocations() const
{
	return (_locations);
}

std::map<int, const std::string>	Server::getErrorPages() const
{
	return (_errorPages);
}