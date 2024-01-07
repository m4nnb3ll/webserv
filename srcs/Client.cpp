/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:10 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/06 22:47:11 by mbouyahy         ###   ########.fr       */
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

void PrintMap(std::map<int, Client *> Map)
{
	std::map<int, Client *>::iterator iter;
	for (iter = Map.begin(); iter != Map.end(); iter++)
	{
		std::cout << "\n<-------------------Info Begin------------------>\n" << std::endl;
        std::cout << "Map Size : " << Map.size() << std::endl;
		std::cout << "Sd : " << iter->first << std::endl;
		std::cout << "RequestLine : " << iter->second->_clientRequest->getRequestLine() << std::endl;
		std::cout << "RequestURI : " << iter->second->_clientRequest->getUri() << std::endl;
		std::cout << "Query : " << iter->second->_clientRequest->getQuery() << std::endl;
		std::cout << "Method : " << iter->second->_clientRequest->getMethod() << std::endl;
		std::cout << "HTTPVersion : " << iter->second->_clientRequest->getHTTPVersion() << std::endl;
		std::cout << "StatusCode : " << iter->second->_clientRequest->getStatusCode() << std::endl;
		std::cout << "Connection : " << iter->second->_clientRequest->getConnection() << std::endl;
		std::cout << "TransferEncoding : " << iter->second->_clientRequest->getTransferEncoding() << std::endl;
		std::cout << "Connection : " << iter->second->_clientRequest->getConnection() << std::endl;
        if (iter->second->_clientRequest->getMethod() == "POST")
        {
            std::cout << "ContentType : " << iter->second->_clientRequest->getContentType() << std::endl;
            std::cout << "ContentLength : " << iter->second->_clientRequest->getContentLength() << std::endl;
        }
		std::cout << "IsFinished : " << iter->second->_clientRequest->isFinished << std::endl;
		std::cout << "\n<-------------------Info End------------------>" << std::endl;

		std::cout << "\n<-------------------Header Begin------------------>\n" << std::endl;
		std::map<std::string, std::string>                          _Header;
		_Header = iter->second->_clientRequest->getHeader();
		for (std::map<std::string, std::string>::iterator Hiter = _Header.begin(); Hiter != _Header.end(); Hiter++)
		{
			std::cout << "Key :   " << Hiter->first << "                                          Value : " << Hiter->second << std::endl;
		}
		std::cout << "\n<-------------------Header End------------------>" << std::endl;

		std::cout << "\n<-------------------Body Begin------------------>\n" << std::endl;
        if (iter->second->_clientRequest->getMethod() == "POST")
		    iter->second->_clientRequest->printVectorOfPairs(iter->second->_clientRequest->getBody());
		
		iter->second->_clientRequest->printVectorOfPairs(iter->second->_clientRequest->files);
		std::cout << "\n<-------------------Body End------------------>\n" << std::endl;
	}
}