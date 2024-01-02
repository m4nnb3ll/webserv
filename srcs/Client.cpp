/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:10 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/01 17:11:59 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	_isSend = false;
	request = NULL;
}

Client::~Client()
{
	// delete ClientRequest;
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
		std::cout << "RequestLine : " << iter->second->request->_getRequestLine() << std::endl;
		std::cout << "RequestURI : " << iter->second->request->_getRequestURI() << std::endl;
		std::cout << "Query : " << iter->second->request->_getQuery() << std::endl;
		std::cout << "Method : " << iter->second->request->_getMethod() << std::endl;
		std::cout << "HTTPVersion : " << iter->second->request->_getHTTPVersion() << std::endl;
		std::cout << "StatusCode : " << iter->second->request->_getStatusCode() << std::endl;
		std::cout << "Connection : " << iter->second->request->_getConnection() << std::endl;
		std::cout << "TransferEncoding : " << iter->second->request->_getTransferEncoding() << std::endl;
		std::cout << "Connection : " << iter->second->request->_getConnection() << std::endl;
        if (iter->second->request->_getMethod() == "POST")
        {
            std::cout << "ContentType : " << iter->second->request->_getContentType() << std::endl;
            std::cout << "ContentLength : " << iter->second->request->_getContentLength() << std::endl;
        }
		std::cout << "IsFinished : " << iter->second->request->_isFinished << std::endl;
		std::cout << "\n<-------------------Info End------------------>" << std::endl;

		std::cout << "\n<-------------------Header Begin------------------>\n" << std::endl;
		std::map<std::string, std::string>                          _Header;
		_Header = iter->second->request->_getHeader();
		for (std::map<std::string, std::string>::iterator Hiter = _Header.begin(); Hiter != _Header.end(); Hiter++)
		{
			std::cout << "Key :   " << Hiter->first << "                                          Value : " << Hiter->second << std::endl;
		}
		std::cout << "\n<-------------------Header End------------------>" << std::endl;

		std::cout << "\n<-------------------Body Begin------------------>\n" << std::endl;
        if (iter->second->request->_getMethod() == "POST")
		    iter->second->request->_printVectorOfPairs(iter->second->request->_getBody());
		
		iter->second->request->_printVectorOfPairs(iter->second->request->_Files);
		std::cout << "\n<-------------------Body End------------------>\n" << std::endl;
	}
}