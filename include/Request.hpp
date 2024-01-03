/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:17:44 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/03 15:34:26 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <ft_common.h>
#include "Config.hpp"

class	Config;
class	Location;

/* Allowed Methods ( GET | POST | DELETE )

The CRLF is the standard line ending sequence in HTTP.
CRLF "\r\n" //Carriage Return && Line Feed
LF "\n" // Line Feed
CR "\r" // Carriage Return

*/

#define FILE_MAX_SIZE 10000000 //10MB (For Testing Only)

class Request
{
    private:
        std::vector<std::pair<std::string, std::string> >           _queryStringParam;
        std::vector<std::pair<std::string, std::string> >           _body;
        std::map<std::string, std::string>                          _header;
        std::string                                                 _method;
        std::string                                                 _query;
        std::string                                                 _requestLine;
        std::string                                                 _httpVersion;
		std::string													_host;
        std::string                                                 _contentType;
        std::string                                                 _uri;
        std::string                                                 _connection;
        std::string                                                 _reasonPhrase;
        std::string                                                 _transferEncoding;
        size_t                                                      _contentLength;
        int                                                         _statusCode;
		Location													*_location;
    public:
        Request();
        ~Request();
        Request(const Request &other);
        Request &operator=(const Request &other);


        //Getters
        std::map<std::string, std::string>                          _getHeader() const;
        std::vector<std::pair<std::string, std::string> >           _getBody() const;
        std::vector<std::pair<std::string, std::string> >           _getQueryStringParam() const;
        std::string                                                 _getTransferEncoding() const;
        std::string                                                 _getRequestURI() const;
        std::string                                                 _getRequestLine() const;
        std::string                                                 _getHTTPVersion() const;
		std::string                                                 getHost() const;
        std::string                                                 _getMethod() const;
        std::string                                                 _getContentType() const;
        std::string                                                 _getConnection() const;
        std::string                                                 _getQuery() const;
        std::string                                                 _getReasonPhrase() const;
        size_t                                                      _getContentLength() const;
        int                                                         _getStatusCode() const;
        Location													*getLocation() const;

        //Setters
        void                                                        _setTransferEncoding(std::string value);
        void                                                        _setRequestURI(std::string value);
        void                                                        _setRequestLine(std::string value);
        void                                                        _setHTTPVersion(std::string value);
        void                                                        _setMethod(std::string value);
        void                                                        _setConnection(std::string value);
        void                                                        _setQuery(std::string value);
        void                                                        _setReasonPhrase(std::string value);
        void                                                        _setStatusCode(int value);
        void                                                        _setContentLength(int value);
        void                                                        _setContentType(std::string value);
        void                                                        _setHeader(std::map<std::string, std::string> value);
        void                                                        _setBody(std::vector<std::pair<std::string, std::string> > value);
        void                                                        _setQueryStringParam(std::vector<std::pair<std::string, std::string> > value);
		void														setLocation(int sd, Config* conf);

        std::vector<std::string>                                    _lines;
        std::string                                                 _allBody;
        std::string                                                 _allHeader;
        size_t                                                      _bodySize;
        std::string                                                 _boundaryBegin;
        std::string                                                 _boundaryEnd;
        bool                                                        _isFinished;
        bool                                                        _status;
        std::vector<std::pair<std::string, std::string> >           _Files;

        //Member Functions
        std::vector<std::string>                                    _splitRequest(std::string data);
        std::string                                                 _fillRequestLine();//change this and work with SearchLine();
        void                                                        _splitLine(std::string Line);
        void                                                        _fillRequestURI();
        bool                                                        _isValidURI();
        void                                                        _specialGetContentType(std::string   Value);
        void                                                        _fillQuery();
        void                                                        _fillQueryStringParam();
        void                                                        _isRequestFinished();
        //Functions For Testing Only
        void                                                        _printVectorOfPairs(std::vector<std::pair<std::string, std::string> >    Body);
};

Request                                                            *FillLines(std::vector<std::string>    SingleRequest);
void                                                                PrintData(std::vector<std::vector<std::string> >  RequestData);
bool                                                                HandleRequest(std::string _readStr, int sd, Config*);

std::string                                                         LandingPage(std::string path);// autoindex

#endif
