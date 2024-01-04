/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asekkak <asekkak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 14:35:22 by asekkak           #+#    #+#             */
/*   Updated: 2024/01/04 15:30:03 by asekkak          ###   ########.fr       */
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
            Cgi(Request *request, Location *config); // sets up env according to the request
            ~Cgi();
            char **_getEnvAsCstrArray() const;
            std::string executeCgi(std::string file); // executes cgi and returns body

    private:
            void _initEnv(Request *request, Location *config);
            std::map<std::string, std::string> _env;
            std::string _body;
};

#endif