/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 14:35:22 by asekkak           #+#    #+#             */
/*   Updated: 2024/01/09 16:32:59 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <ft_common.h>
#include "Request.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Config.hpp"

#define BUFFER_CGI 2048

class Request;
class Location;
class Response;
class Config;

class Cgi
{
    public:
			Cgi(Response* response, std::string cgiPath, std::string filePath); // sets up env according to the request
            ~Cgi();
            char**		_getEnvAsCstrArray() const;
            std::string	execute(); // executes cgi and returns body

    private:
            void								_initEnv();
			std::string							_addContentLength(const std::string&);
            std::map<std::string, std::string>	_env;
            std::string							_body;
			std::string							_cgiPath;
			std::string							_filePath;
			Response							*_response;
};

#endif