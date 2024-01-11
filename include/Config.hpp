/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:31:03 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/11 19:01:13 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_common.h>
#include "Server.hpp"
#include "Printers.hpp"
#include "Client.hpp"

extern std::map<std::string, std::string>	g_mimeTypes;

class	Printers;
class	ServersSocket;
class	Client;

class	Config
{
public :
	Config(const std::string &);
	~Config();
	void									run();
	static struct sockaddr_in				strToSockaddrin(const std::string& port);
	// GETTERS
	std::map<int, Client*>					getSdToClient() const;
	ServersSocket*							getServersSocket(int) const;
	// SETTERS
	void									insertToSdToClient(std::pair<int, Client*>);

private:
	std::ifstream							_configFile;
	std::map<std::string, ServersSocket*>	_portToServersSocket;
	std::map<int, ServersSocket*>			_sdToServersSocket;
	std::map<int, Client*>					_sdToClient;
	std::vector<struct pollfd>				_pollFds;

	// METHODS
	void									_openConfig(const std::string &);
	std::string&							_ignoreComments(std::string &);
	std::istream&							_getEntry(std::ifstream&, std::string&);
	void									_parseLocation(std::istringstream& , Server*);
	void									_addPollfd(int sd, short events);
	void									_rmPollfd(int sd);
	void									_initSockets();
	void									_unplugSocket(ServersSocket *sS);
	void									_readRequest(int sd);
	void									_sendResponse(int sd);
	bool									_portExists(const std::string& s);
	std::string								_extractListen(std::istringstream& iss);
	void									_pruneSocket(int sd, ServersSocket* sS);
	void									_addClient(int sd, ServersSocket* sS);

	// PARSERS >
	void									_parseServer();
	void									_parseListen(std::istringstream&, Server*);
	void									_parseServerNames(std::istringstream&, Server*);
	void									_setRoot(std::istringstream&, Server*);
};
