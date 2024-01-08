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

#pragma once

#include <ft_common.h>
#include "Request.hpp"
class	Request;
class	Response;

class   Client
{
    public:
        Client(std::string);
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);

		Request		*getRequest() const;
		Response	*getResponse() const;
		void		deleteRequest();
		void		createRequest(std::string);
		std::string	createResponse();

        Request		*_request;
        bool		_isSend;
};

void PrintMap(std::map<int, Client *> Map);//For Testing Only
