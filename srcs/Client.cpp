/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:10 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/05 16:14:45 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	_isSend = false;
	_clientRequest = NULL;
}

Client::~Client()
{
	// delete ClientRequest;
}

Response*	Client::getResponse() const
{
	return (_clientRequest->getResponse());
}

Client::Client(const Client &other){ (void)other; }

Client &Client::operator=(const Client &other)
{
    if (this != &other){}
    return (*this);
}

void PrintMap(std::map<int, Client *> *Map)
{
	std::map<int, Client *>::iterator iter;
	for (iter = Map->begin(); iter != Map->end(); iter++)
	{
		std::cout << "\n<-------------------Info Begin------------------>\n" << std::endl;
        std::cout << "Map Size : " << Map->size() << std::endl;
		std::cout << "Sd : " << iter->first << std::endl;
		std::cout << "RequestLine : " << iter->second->_clientRequest->_getRequestLine() << std::endl;
		std::cout << "RequestURI : " << iter->second->_clientRequest->_getUri() << std::endl;
		std::cout << "Query : " << iter->second->_clientRequest->_getQuery() << std::endl;
		std::cout << "Method : " << iter->second->_clientRequest->getMethod() << std::endl;
		std::cout << "HTTPVersion : " << iter->second->_clientRequest->_getHTTPVersion() << std::endl;
		std::cout << "StatusCode : " << iter->second->_clientRequest->_getStatusCode() << std::endl;
		std::cout << "Connection : " << iter->second->_clientRequest->_getConnection() << std::endl;
		std::cout << "TransferEncoding : " << iter->second->_clientRequest->_getTransferEncoding() << std::endl;
		std::cout << "Connection : " << iter->second->_clientRequest->_getConnection() << std::endl;
    if (iter->second->_clientRequest->getMethod() == "POST")
    {
        std::cout << "ContentType : " << iter->second->_clientRequest->_getContentType() << std::endl;
        std::cout << "ContentLength : " << iter->second->_clientRequest->_getContentLength() << std::endl;
    }
		std::cout << "IsFinished : " << iter->second->_clientRequest->_isFinished << std::endl;
		std::cout << "\n<-------------------Info End------------------>" << std::endl;

		std::cout << "\n<-------------------Header Begin------------------>\n" << std::endl;
		std::map<std::string, std::string>                          _Header;
		_Header = iter->second->_clientRequest->_getHeader();
		for (std::map<std::string, std::string>::iterator Hiter = _Header.begin(); Hiter != _Header.end(); Hiter++)
		{
			std::cout << "Key :   " << Hiter->first << "                                          Value : " << Hiter->second << std::endl;
		}
		std::cout << "\n<-------------------Header End------------------>" << std::endl;

		std::cout << "\n<-------------------Body Begin------------------>\n" << std::endl;
        if (iter->second->_clientRequest->_getMethod() == "POST")
		    iter->second->_clientRequest->_printVectorOfPairs(iter->second->_clientRequest->_getBody());
		
		iter->second->_clientRequest->_printVectorOfPairs(iter->second->_clientRequest->_Files);
		std::cout << "\n<-------------------Body End------------------>\n" << std::endl;
	}
}