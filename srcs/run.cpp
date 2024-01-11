/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:10:00 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/11 22:35:03 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Request.hpp"

void Config::_readRequest(int sd)
{
	char		buffer[BUFFER_SIZE];
	std::string	strToPass;
	int			ret;

    memset(buffer, 0, BUFFER_SIZE);
	ret = recv(sd, buffer, BUFFER_SIZE, 0);
	if (!ret || ret == -1) {
		_rmPollfd(sd);
		std::cout << YELLOW << "[Attention]: Client with socket: " << sd << " disconnected(Reading)!" << RESET_COLOR;
		return;
	}
	strToPass="";
	strToPass.append(buffer, ret); // Didn't use += to no ignore chars after '\0'
	if (_sdToClient[sd])
	{
		_sdToClient[sd]->getRequest()
			?	_sdToClient[sd]->getRequest()->appendStr(strToPass)
			:	_sdToClient[sd]->createRequest(strToPass);
	}
	else
		_sdToClient[sd] = new Client(strToPass);
	_sdToClient[sd]->getRequest()->handleRequest(sd, this);
}

void Config::_sendResponse(int sd)
{
	Client	*client;

	// Check if the client object exists
	if (_sdToClient.find(sd) == _sdToClient.end()) return ;

	client = _sdToClient[sd];
	if (client && client->getRequest() && client->getRequest()->isFinished)
	{
		std::string	content = client->createResponse();
		int ret = send(sd, content.c_str(), content.size(), 0);
		if (ret == -1) {
			_rmPollfd(sd);
			delete	_sdToClient[sd];
			_sdToClient[sd] = NULL;
			std::cout << YELLOW << "[Attention]: Client with socket: " << sd << " disconnected(Writing)!\n" << RESET_COLOR;
		}
		client->deleteRequest();
	}
}

void	Config::_pruneSocket(int sd, ServersSocket* sS)
{
	if ((sS = _sdToServersSocket[sd]) && sS->getFd() == sd) _unplugSocket(sS);
	_rmPollfd(sd);
	if (_sdToClient.find(sd) != _sdToClient.end())
	{
		delete (_sdToClient[sd]);
		_sdToClient[sd] = NULL;
	}
	std::cout << YELLOW << "[WARNING]: Pruning the socket with fd: " << sd << " disconnected(Writing)!\n" << RESET_COLOR;
}

void	Config::_addClient(int sd, ServersSocket* sS)
{
	sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd = accept(sd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd < 0) return;
	_sdToServersSocket[client_fd] = sS;
	_addPollfd(client_fd, POLLIN | POLLOUT);
}

void Config::run()
{
	int				sd;
	ServersSocket* 	sS;

	while (!g_sigint) // to give program time to clean up
	{
		if ((poll(_pollFds.data(), _pollFds.size(), -1)) <= 0) continue ;
		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			sd = _pollFds[i].fd;
			if ((_pollFds[i].revents & POLLERR) || _pollFds[i].revents & POLLHUP)
				_pruneSocket(sd, sS);
			else if (_pollFds[i].revents & POLLIN)
				((sS = _sdToServersSocket[sd]) && (sS->getFd() == sd))
					?	_addClient(sd, sS)
					:	_readRequest(sd);
			else if (_pollFds[i].revents & POLLOUT) // send response to the client
				_sendResponse(sd);
		}
	}
}