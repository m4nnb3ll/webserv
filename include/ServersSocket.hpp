/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:31:23 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/07 22:31:24 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_common.h"
#include "Server.hpp"

class	Server;

class	ServersSocket
{
public:
	ServersSocket(Server*);
	~ServersSocket();
	void						setFd(int);
	int							getFd() const;
	void						addServer(Server *s);
	const std::vector<Server*>&	getServers() const;
	Server*						getServer(std::string host) const;
private:
	int							_sd;
	std::vector<Server*>		_servers;
};
