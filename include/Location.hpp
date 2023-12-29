#pragma once

#include <ft_common.h>
#include "Server.hpp"

class	Server;

class Location
{
	private:
		std::string							_path;
		std::string							_rootPath;
		std::vector<std::string>			_indexes;
		bool								_autoIndex;
		std::vector<std::string>			_allowMethods;
		bool								_redirect;
		std::string							_redirectPath;
		int									_redirectCode;
		Server*								_server;
		std::map<std::string, std::string>	_cgi;
		std::string							_uploadDir;
	public :
		Location(Server*);
		Location(const Location &);
		~Location();
		Location &operator=(const Location &);

		void										setPath(std::istringstream &iss);
		void										setRootPath(std::istringstream &iss);
		void										setUploadDir(std::istringstream &iss);
		void										addIndex(std::istringstream &iss);
		void										setAutoIndex(std::istringstream &iss);
		void										setAllowMethods(std::istringstream &iss);
		void										setRedirect(std::istringstream &iss);
		void										addCgi(std::istringstream& iss);

		const std::string&							getPath() const;
		std::string									getRootPath() const;
		std::string									getUploadDir() const;
		const std::vector<std::string>&				getIndexes() const;
		const bool&									getAutoIndex() const;
		const std::vector<std::string>&				getAllowMethods() const;
		const std::vector<std::string>&				getServerNames() const;
		const std::string&							getRedirectPath() const;
		const bool&									getRedirect() const;
		const int&									getRedirectCode() const;
		std::map<std::string, std::string> 			getCgi()  ;
};
