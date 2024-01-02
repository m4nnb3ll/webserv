/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:17:44 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/02 13:13:13 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <ft_common.h>
#include "Client.hpp"
#include "Response.hpp"

class Client;

/* Allowed Methods ( GET | POST | DELETE )

The CRLF is the standard line ending sequence in HTTP.
CRLF "\r\n" //Carriage Return && Line Feed
LF "\n" // Line Feed
CR "\r" // Carriage Return

*/

#define FILE_MAX_SIZE 10000000 //10MB (For Testing Only)

class Request
{
public:
	Request();
	~Request();

	//Getters
	std::map<std::string, std::string>                          getHeader() const;
	std::vector<std::pair<std::string, std::string> >           getBody() const;
	std::vector<std::pair<std::string, std::string> >           getQueryStringParam() const;
	std::string                                                 getTransferEncoding() const;
	std::string                                                 getRequestURI() const;
	std::string                                                 getRequestLine() const;
	std::string                                                 getHTTPVersion() const;
	std::string                                                 getMethod() const;
	std::string                                                 getContentType() const;
	std::string                                                 getConnection() const;
	std::string                                                 getQuery() const;
	std::string                                                 getReasonPhrase() const;
	size_t                                                      getContentLength() const;
	int                                                         getStatusCode() const;
	
	//Setters
	void                                                        setTransferEncoding(std::string value);
	void                                                        setRequestURI(std::string value);
	void                                                        setRequestLine(std::string value);
	void                                                        setHTTPVersion(std::string value);
	void                                                        setMethod(std::string value);
	void                                                        setConnection(std::string value);
	void                                                        setQuery(std::string value);
	void                                                        setReasonPhrase(std::string value);
	void                                                        setStatusCode(int value);
	void                                                        setContentLength(int value);
	void                                                        setContentType(std::string value);
	void                                                        setHeader(std::map<std::string, std::string> value);
	void                                                        setBody(std::vector<std::pair<std::string, std::string> > value);
	void                                                        setQueryStringParam(std::vector<std::pair<std::string, std::string> > value);
	void														appendStr(std::string);
	
	std::vector<std::string>                                    lines;
	std::string                                                 allBody;
	std::string                                                 allHeader;
	size_t                                                      bodySize;
	std::string                                                 boundaryBegin;
	std::string                                                 boundaryEnd;
	bool                                                        isFinished;
	bool                                                        status;
	std::vector<std::pair<std::string, std::string> >           Files;

	//Member Functions
	std::vector<std::string>                                    splitRequest(std::string data);
	void                                                        splitLine(std::string Line);
	void                                                        fillRequestURI();
	bool                                                        isValidURI();
	void                                                        specialGetContentType(std::string   Value);
	std::string                                                 fillRequestLine();//change this and work with SearchLine();
	void                                                        fillQueryStringParam();
	void                                                        fillQuery();
	void                                                        isRequestFinished();
	void														update();
	//Functions For Testing Only
	void                                                        printVectorOfPairs(std::vector<std::pair<std::string, std::string> >    Body);

private:
	std::string                                                 _requestLine;
	std::string                                                 _method;
	int                                                         _statusCode;
	std::string                                                 _query;
	std::string                                                 _httpVersion;
	std::string                                                 _contentType;
	std::string                                                 _requestURI;
	std::string                                                 _connection;
	std::string                                                 _reasonPhrase;
	std::string                                                 _transferEncoding;
	size_t                                                      _contentLength;
	std::vector< std::pair<std::string, std::string>>           _queryStringParam;
	std::map<std::string, std::string>                          _header;
	std::vector< std::pair<std::string, std::string>>           _body;
	Response													*_responsePtr;
	std::string													_reqStr;
	bool														_headerComplete;
	bool														_bodyComplete;
	// Private Methods
	Request*													_updateHeader();
	Request*													_updateBody();
};

Request                                                            *parseRequest(std::vector<std::string>    SingleRequest);
bool                                                                HandleRequest(std::string _readStr, int sd, std::map<int, Client *>	*ClientsInformation);

void                                                                PrintData(std::vector<std::vector<std::string> >  RequestData);
// std::string                                                         LandingPage(std::string path);// autoindex

#endif
