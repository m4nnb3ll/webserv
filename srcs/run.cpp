/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:10:00 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/07 21:53:11 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Request.hpp"

/*TEST*/
template <typename K, typename V>
void printMap(const std::map<K, V>& inputMap)
{
    std::cout << "Map Contents:" << std::endl;
    typename std::map<K, V>::const_iterator it;
    for (it = inputMap.begin(); it != inputMap.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }
}
/*TEST*/

void Config::_readRequest(int sd)
{

	// The server-name WILL be checked after reading the request

    char	buffer[BUFFER_SIZE];
	int		ret;

    memset(buffer, 0, BUFFER_SIZE);
	ret = recv(sd, buffer, BUFFER_SIZE - 1, 0);

	if (!ret || ret == -1) {
		_rmPollfd(sd);
		std::cout << "reading: client[" << sd <<"] disconnected" << std::endl;
		return;
	}
	std::cout << "The fist time!" << std::endl;

	// make sure the delete the client after the poll quit
	if (_sdToClient[sd])
	{
		_sdToClient[sd]->getRequest()
			?	_sdToClient[sd]->getRequest()->appendStr(buffer)
			:	_sdToClient[sd]->createRequest(buffer);
	}
	else
		_sdToClient[sd] = new Client(buffer);
	_sdToClient[sd]->getRequest()->handleRequest(sd, this);
}

void Config::_sendResponse(int sd)
{
	Client								*client;
	std::map<int, Client *>::iterator   iter;
	
	iter = _sdToClient.find(sd);
	if (iter == _sdToClient.end()) return ;

	client = _sdToClient[sd];
	if (client->getRequest() && client->getRequest()->isFinished)
	{
		std::string	content = client->createResponse();
		int ret = send(sd, content.c_str(), content.size(), 0);
		if (ret == -1) {
			_rmPollfd(sd);
			_sdToClient.erase(sd);
			std::cout << "writing: client[" << sd <<"] disconnected" << std::endl;
			return ;
		}
		client->deleteRequest();
	}
}

void Config::run()
{
	Printers::print_serversSockets(_portToServersSocket);

	ServersSocket* 				sS;

	while (!g_sigint)
	{
		if ((poll(_pollFds.data(), _pollFds.size(), -1)) <= 0)
			continue;
		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			int sd = _pollFds[i].fd;
			if ((_pollFds[i].revents & POLLERR)
				|| _pollFds[i].revents & POLLHUP)
			{
				if ((sS = _sdToServersSocket[sd]) && sS->getFd() == sd)
					_unplugSocket(sS);
				_rmPollfd(sd);
				std::cerr << "POLL QUIT " << sd << std::endl;
			}
			else if (_pollFds[i].revents & POLLIN)
			{
				if ((sS = _sdToServersSocket[sd]) && sS->getFd() == sd)
				{
					sockaddr_in client_addr;
					socklen_t client_addr_len = sizeof(client_addr);
					int client_fd = accept(sd, (struct sockaddr *)&client_addr, &client_addr_len);
					if (client_fd < 0) continue;
					_sdToServersSocket[client_fd] = sS;
					_addPollfd(client_fd, POLLIN | POLLOUT);
				}
				else // read request from client
					_readRequest(sd);
			}
			else if (_pollFds[i].revents & POLLOUT) // send response to the client
			{
				_sendResponse(sd);
			}
		}
	}
}