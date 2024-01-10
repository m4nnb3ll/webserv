/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:10 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/10 15:48:42 by abelayad         ###   ########.fr       */
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

void PrintMap(std::map<int, Client *> Map)
{
	std::map<int, Client *>::iterator iter;
	for (iter = Map.begin(); iter != Map.end(); iter++)
	{
		std::cout << "\n<-------------------Info Begin------------------>\n" << std::endl;
        std::cout << "Map Size : " << Map.size() << std::endl;
		std::cout << "Sd : " << iter->first << std::endl;
		std::cout << "RequestLine : " << iter->second->_request->getRequestLine() << std::endl;
		std::cout << "RequestURI : " << iter->second->_request->getUri() << std::endl;
		std::cout << "Query : " << iter->second->_request->getQuery() << std::endl;
		std::cout << "Method : " << iter->second->_request->getMethod() << std::endl;
		std::cout << "HTTPVersion : " << iter->second->_request->getHTTPVersion() << std::endl;
		std::cout << "StatusCode : " << iter->second->_request->getStatusCode() << std::endl;
		std::cout << "Connection : " << iter->second->_request->getConnection() << std::endl;
		std::cout << "TransferEncoding : " << iter->second->_request->getTransferEncoding() << std::endl;
		std::cout << "Connection : " << iter->second->_request->getConnection() << std::endl;
        if (iter->second->_request->getMethod() == "POST")
        {
            std::cout << "ContentType : " << iter->second->_request->getContentType() << std::endl;
            std::cout << "ContentLength : " << iter->second->_request->getContentLength() << std::endl;
        }
		std::cout << "IsFinished : " << iter->second->_request->isFinished << std::endl;
		std::cout << "\n<-------------------Info End------------------>" << std::endl;

		std::cout << "\n<-------------------Header Begin------------------>\n" << std::endl;
		std::map<std::string, std::string>                          _Header;
		_Header = iter->second->_request->getHeader();
		for (std::map<std::string, std::string>::iterator Hiter = _Header.begin(); Hiter != _Header.end(); Hiter++)
		{
			std::cout << "Key :   " << Hiter->first << "                                          Value : " << Hiter->second << std::endl;
		}
		std::cout << "\n<-------------------Header End------------------>" << std::endl;

		std::cout << "\n<-------------------Body Begin------------------>\n" << std::endl;
        if (iter->second->_request->getMethod() == "POST")
		    iter->second->_request->printVectorOfPairs(iter->second->_request->getBody());
		
		iter->second->_request->printVectorOfPairs(iter->second->_request->files);
		std::cout << "\n<-------------------Body End------------------>\n" << std::endl;
	}
}