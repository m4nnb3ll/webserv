#pragma once

#include "ft_common.h"
#include "Location.hpp"
#include "ServersSocket.hpp"

class	Location;
class	ServersSocket;

class	Printers
{
public:
	static void	print_with_color(const std::string& color, const std::string& s);
	static void	print_addr(struct sockaddr_in addr);
	static void	print_serverNames(std::vector<std::string> server_names);
	static void	print_error_pages(const std::map<int, const std::string> error_pages);
	static void	print_mbs(size_t mbs);
	static void	print_err(const char * strerr);
	static void	print_indexes(const std::vector<std::string>& indexes);
	static void	print_allow_methods(const std::vector<std::string>& allowMethods);
	static void	print_redirect(Location* location);
	static void	print_locations(const Server *srv);
	static void	print_servers(ServersSocket *sS);
	static void	print_serversSockets(const std::map<std::string, ServersSocket*> mappa);
private:
	Printers();
};
