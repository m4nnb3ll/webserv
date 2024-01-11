/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:10 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/11 19:03:21 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(std::string buffer)
	:	_request(new Request(buffer)), _isSend(false)
{}

Client::~Client()
{
	delete	_request;
}

void	Client::createRequest(std::string buffer)
{
	delete	(_request);
	_request = new Request(buffer);
}

void	Client::deleteRequest()
{
	delete _request;
	_request = NULL;
}

std::string	Client::createResponse()
{
	std::string	content;

	Response* response = new Response(_request);
	content = response->getContent();
	delete response;
	return (content);
}

Request*	Client::getRequest() const
{
	return (_request);
}

Response*	Client::getResponse() const
{
	return (_request->getResponse());
}

Client::Client(const Client &other){ (void)other; }

Client &Client::operator=(const Client &other)
{
    if (this != &other){}
    return (*this);
}
