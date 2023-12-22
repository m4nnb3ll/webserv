#pragma once

#include <ft_common.h>
#include "Server.hpp"
#include "Printers.hpp"

#include "Client.hpp"//added by mbouyahy

class	Printers;
class	ServersSocket;

class	Config
{
public :
	
	Config(const std::string &);
	~Config();
	void						run();
	static struct sockaddr_in	strToSockaddrin(const std::string& port);
private:
	std::ifstream							_configFile;
	std::map<std::string, ServersSocket*>	_portToServersSocket;
	std::map<int, ServersSocket*>			_sdToServersSocket;//sd: socket descriptor
	// std::map<int, Client*>					_sdToClients;
	std::vector<struct pollfd>				_pollFds;

	std::string								_readStr;

	// METHODS
	void			_openConfig(const std::string &);
	std::string&	_ignoreComments(std::string &);
	std::istream&	_getEntry(std::ifstream&, std::string&);
	void			_parseLocation(std::istringstream& , Server*);
	void			_addPollfd(int sd, short events);
	void			_rmPollfd(int sd);
	void			_initSockets();
	void			_unplugSocket(ServersSocket *sS);
	void			_readRequest(int sd);
	void			_sendResponse(int sd);
	bool			_portExists(const std::string& s);
	std::string		_getListen(std::istringstream& iss);

	// PARSERS >
	void			_parseServer();
	void			_parseListen(std::istringstream&, Server*);
	void			_parseServerNames(std::istringstream&, Server*);
	void			_setRoot(std::istringstream&, Server*);
};

// std::map<int, Client*>					_sdToClients;


// class	Client
// {
// 	_readStr;
// 	Request;
// 	Response;
// };



