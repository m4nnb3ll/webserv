/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:23 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/27 18:44:55 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Requests.hpp"
class Request;

class   Client
{
    public:
        Client();
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);

        Request    *ClientRequest;//change this into vector of requests if needed
};

void PrintMap(std::__1::map<int, Client *> *_Map);

#endif