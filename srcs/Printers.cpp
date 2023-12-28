#include "Printers.hpp"

void	Printers::print_with_color(const std::string& color, const std::string& s)
{
	std::cout << color << s << RESET_COLOR << std::endl;
}

// void	Printers::print_addr(struct sockaddr_in addr)
// {
// 	Printers::print_with_color(YELLOW, "The address is:");
// 	struct sockaddr_in temp = addr;
// 	std::cout << inet_ntoa(temp.sin_addr)
// 	<< ":" << ntohs(temp.sin_port) << " ";
// 	std::cout << std::endl;
// }

// void	Printers::print_addrs(const std::vector<struct sockaddr_in> addrs)
// {
// 	Printers::print_with_color(YELLOW, "The addresses are:");
// 	for (unsigned int i = 0; i < addrs.size(); i++)
// 	{
// 		struct sockaddr_in temp = addrs[i];
// 		std::cout << inet_ntoa(temp.sin_addr)
// 		<< ":" << ntohs(temp.sin_port) << " ";
// 	}
// 	std::cout << std::endl;
// }

void	Printers::print_serverNames(std::vector<std::string> server_names)
{
	size_t i;
	Printers::print_with_color(GREEN, "server_names: ");
	for (i = 0; i < server_names.size(); i++)
		std::cout << server_names[i] << " ";
	std::cout << std::endl;
}

void	Printers::print_error_pages(const std::map<int, const std::string> error_pages)
{
	Printers::print_with_color(GREEN, "The error_pages are:");
    for (std::map<int, const std::string>::const_iterator it = error_pages.begin();
		it != error_pages.end(); ++it)
        std::cout << "error: " << it->first << ", page: " << it->second << std::endl;
}

void	Printers::print_mbs(size_t mbs)
{
	std::cout << GREEN << "The maxBodySize is: " << RESET_COLOR << mbs << std::endl;
}

void	Printers::print_err(const char * strerr)
{
	std::cerr << strerr << std::endl;
	std::cout << "I get in here" << std::endl;
	exit(42);
}

void	Printers::print_indexes(const std::vector<std::string>& indexes)
{
	std::cout << "Indexes: ";
	for (size_t i = 0; i < indexes.size(); i++)
		std::cout << indexes[i] + " ";
	std::cout << std::endl;
}

void	Printers::print_allow_methods(const std::vector<std::string>& allowMethods)
{
	std::cout << "The allow_methods: ";
	for (size_t i = 0; i < allowMethods.size(); i++)
		std::cout << allowMethods[i] + " ";
	std::cout << std::endl;
}

void	Printers::print_redirect(Location* location)
{
	std::cout << "The redirect: " << std::endl;
	std::cout << "\tredirect?: " << (location->getRedirect() ? "True" : "False") << std::endl;
	std::cout << "\tredirectPath: " << location->getRedirectPath() << std::endl;
	std::cout << "\tredirectCode: " << location->getRedirectCode() << std::endl;
}

void	Printers::print_locations(const Server *srv)
{
	const std::vector<Location*>& locations = srv->getLocations();

	Printers::print_with_color(GREEN, "The locations are: ");
	for (size_t i = 0; i < locations.size(); i++)
	{
		std::cout << BLUE << "-------------------" << i << "-------------------" << RESET_COLOR << std::endl;
		std::cout << "Path: " << locations[i]->getPath() << std::endl;
		std::cout << "RootPath: " << locations[i]->getRootPath() << std::endl;
		std::cout << "UploadDir: " << locations[i]->getUploadDir() << std::endl;
		print_indexes(locations[i]->getIndexes());
		std::cout << "AutoIndex: " << locations[i]->getAutoIndex() << std::endl;
		print_allow_methods(locations[i]->getAllowMethods());
		print_redirect(locations[i]);
	}
	std::cout << BLUE << "---------------------------------------";
	std::cout << GREEN << "<Locations end" << RESET_COLOR << std::endl;
}

void	Server::print() const
{
	Printers::print_serverNames(_serverNames);
	Printers::print_error_pages(_errorPages);
	Printers::print_mbs(_maxBodySize);
	Printers::print_locations(this);
}

void	Printers::print_servers(ServersSocket *sS)
{
	const std::vector<Server*>& servers(sS->getServers());
	for (size_t i = 0; i < servers.size(); i++)
	{
		std::ostringstream oss;
		oss << "Server " << i << "------------------------------";
		Printers::print_with_color(YELLOW, oss.str());
		servers[i]->print();
		oss.str(std::string());
		oss << "------------------------------end of Server " << i;
		Printers::print_with_color(YELLOW, oss.str());
	}
}

void	Printers::print_serversSockets(const std::map<std::string, ServersSocket*> mappa)
{
	for (std::map<std::string, ServersSocket*>::const_iterator it = mappa.begin();
		it != mappa.end(); it++)
	{
		std::cout << MAGENTA << "SOCKET>>> ";
		std::cout << "port: " << it->first << ", servSock_ptr: " << it->second << std::endl;
		Printers::print_servers(it -> second);
	}
}
