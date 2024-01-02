/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asekkak <asekkak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:23 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/02 13:47:07 by asekkak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ft_common.h>
#include "Request.hpp"
#include "Response.hpp"
#include "Location.hpp"
class Request;
class Location;
class Response;

class   Client
{
    public:
        Client();
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);

        Request     *_clientRequest;//change this into vector of requests if needed
        Response     *_clientResponse;//change this into vector of requests if needed
        bool        _isSend;
        bool        _isFound;
};

void PrintMap(std::map<int, Client *> *Map);
void PrintMap(std::map<int, Client *> *_Map);
void get_response(int sd, Client *client ,const std::vector<Location *> &serverLocation , std::map<int, Client *> ClientsInformation);
void sendResponseTest(std::string message, int sd, int isText);
std::string cgi(std::string file, int sd, Location *serverLocation, std::map<int, Client *> ClientsInformation);
void post_response(int sd, Client *client ,const std::vector<Location *> &serverLocation , std::map<int, Client *> ClientsInformation);

#endif