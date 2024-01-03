#include "ServersSocket.hpp"

ServersSocket::ServersSocket(Server *s)
	: _sd(-1)
{
	_servers.push_back(s);
}

ServersSocket::~ServersSocket()
{
	for (size_t i = 0; i < _servers.size(); i++)
		delete _servers[i];//heap-use-after-free!
}

void	ServersSocket::setFd(int sd)
{
	_sd = sd;
}

int	ServersSocket::getFd() const
{
	return (_sd);
}

void	ServersSocket::addServer(Server *s)
{
	_servers.push_back(s);
}

const std::vector<Server*>&	ServersSocket::getServers() const
{
	return (_servers);
}

Server*	ServersSocket::getServer(std::string host) const
{
	std::istringstream	iSS(host);
	std::string			hostname;
	Server*				server;

	std::getline(iSS, hostname, ':');
	for (size_t i = 0; i < _servers.size(); i++)
	{
		const std::vector<std::string>& serversNames = _servers[i]->getServerNames();
		for (size_t j = 0; j < serversNames.size(); j++)
			if (serversNames[j] == host) return (_servers[i]);
	}
	return (_servers[0]);
}