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
		void									addLocation(Location *location);
		const std::vector<std::string>&			getServerNames() const;
		const std::vector<Location*>&			getLocations() const;
	private:
		std::vector<std::string>				_serverNames;
		std::map<int, const std::string>		_errorPages;
		std::vector<Location*>					_locations;
		size_t									_maxBodySize;
};
