/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:23 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/31 20:28:28 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ft_common.h>
#include "Request.hpp"
class Request;

class   Client
{
    public:
        Client();
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);

        Request     *_clientRequest;//change this into vector of requests if needed
        bool        _isSend;
};

void PrintMap(std::map<int, Client *> *Map);

#endif