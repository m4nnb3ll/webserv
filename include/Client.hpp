/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:23 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/21 21:43:04 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
#define Client_HPP

#include "Requests.hpp"
class Requests;

class   Client
{
    public:
        Client();
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);
        Requests    *ClientRequest;
};

void PrintMap(std::__1::map<int, Client *> *_Map);

#endif