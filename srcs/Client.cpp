/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:10 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/30 19:35:32 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	isSend = false;
	ClientRequest = NULL;
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

void PrintMap(std::map<int, Client *> *_Map)
{
	std::map<int, Client *>::iterator iter;
	for (iter = _Map->begin(); iter != _Map->end(); iter++)
	{
		std::cout << "\n<-------------------Info Begin------------------>\n" << std::endl;
        std::cout << "Map Size : " << _Map->size() << std::endl;
		std::cout << "Sd : " << iter->first << std::endl;
		std::cout << "RequestLine : " << iter->second->ClientRequest->GetRequestLine() << std::endl;
		std::cout << "RequestURI : " << iter->second->ClientRequest->GetRequestURI() << std::endl;
		std::cout << "Query : " << iter->second->ClientRequest->GetQuery() << std::endl;
		std::cout << "Method : " << iter->second->ClientRequest->GetMethod() << std::endl;
		std::cout << "HTTPVersion : " << iter->second->ClientRequest->GetHTTPVersion() << std::endl;
		std::cout << "StatusCode : " << iter->second->ClientRequest->GetStatusCode() << std::endl;
		std::cout << "Connection : " << iter->second->ClientRequest->GetConnection() << std::endl;
		std::cout << "TransferEncoding : " << iter->second->ClientRequest->GetTransferEncoding() << std::endl;
		std::cout << "Connection : " << iter->second->ClientRequest->GetConnection() << std::endl;
        if (iter->second->ClientRequest->GetMethod() == "POST")
        {
            std::cout << "ContentType : " << iter->second->ClientRequest->GetContentType() << std::endl;
            std::cout << "ContentLength : " << iter->second->ClientRequest->GetContentLength() << std::endl;
        }
		std::cout << "IsFinished : " << iter->second->ClientRequest->_isFinished << std::endl;
		std::cout << "\n<-------------------Info End------------------>" << std::endl;

		std::cout << "\n<-------------------Header Begin------------------>\n" << std::endl;
		std::map<std::string, std::string>                          _Header;
		_Header = iter->second->ClientRequest->GetHeader();
		for (std::map<std::string, std::string>::iterator Hiter = _Header.begin(); Hiter != _Header.end(); Hiter++)
		{
			std::cout << "Key :   " << Hiter->first << "                                          Value : " << Hiter->second << std::endl;
		}
		std::cout << "\n<-------------------Header End------------------>" << std::endl;

		std::cout << "\n<-------------------Body Begin------------------>\n" << std::endl;
        if (iter->second->ClientRequest->GetMethod() == "POST")
		    iter->second->ClientRequest->PrintVectorOfPairs(iter->second->ClientRequest->GetBody());
		
		iter->second->ClientRequest->PrintVectorOfPairs(iter->second->ClientRequest->_Files);
		std::cout << "\n<-------------------Body End------------------>\n" << std::endl;
	}
}