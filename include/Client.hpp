/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:23 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/06 23:05:16 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ft_common.h>
#include "Request.hpp"
class	Request;
class	Response;

class   Client
{
    public:
        Client();
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);

		Response	*getResponse() const;

        Request     *_clientRequest;
        bool        _isSend;
};

void PrintMap(std::map<int, Client *> Map);//For Testing Only

#endif