/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:31:21 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/11 18:38:43 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_common.h>
#include "Location.hpp"
#include "ServersSocket.hpp"
#include "Config.hpp"

class	Config;
class	Location;

class	Server
{
    public:
        Server();
		~Server();
		void									print() const;
		void									addServerName(const std::string& serverName);
		void									setErrorPages(int errCode, const std::string& errorPage);
		void									setMaxBodySize(size_t maxBodySize);
		size_t									getMaxBodySize() const;
		void									addLocation(Location *location);
		const std::vector<std::string>&			getServerNames() const;
		const std::vector<Location*>&			getLocations() const;
		std::map<int, const std::string>		getErrorPages() const;
	private:
		std::vector<std::string>				_serverNames;
		std::map<int, const std::string>		_errorPages;
		std::vector<Location*>					_locations;
		size_t									_maxBodySize;
};
