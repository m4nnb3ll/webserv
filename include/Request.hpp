/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:17:44 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/30 19:33:35 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

//change the name of file to HttpRequest or the reverse
#include <ft_common.h>
#include "Client.hpp"

class Client;

//Allowed Methods ( GET | POST | DELETE )

//The CRLF is the standard line ending sequence in HTTP.
// CRLF "\r\n" //Carriage Return && Line Feed
// LF "\n" // Line Feed
// CR "\r" // Carriage Return

class Request
{
    private:
        std::vector<std::pair<std::string, std::string> >           _body;
        std::vector<std::pair<std::string, std::string> >           _queryStringParam;
        std::map<std::string, std::string>                          _header;
        std::string                                                 _method;
        size_t                                                      _contentLength;
        int                                                         _statusCode;
        std::string                                                 _reasonPhrase;
        std::string                                                 _transferEncoding;
        std::string                                                 _requestLine;
        std::string                                                 _httpVersion;
        std::string                                                 _requestURI;
        std::string                                                 _contentType;
        std::string                                                 _connection;
        std::string                                                 _query;
    public:
        Request();
        ~Request();
        Request(const Request &other);
        Request &operator=(const Request &other);


        //Getters
        std::map<std::string, std::string>                          GetHeader() const;
        std::vector<std::pair<std::string, std::string> >           GetBody() const;
        std::vector<std::pair<std::string, std::string> >           GetQueryStringParam() const;
        int                                                         GetStatusCode() const;
        size_t                                                      GetContentLength() const;
        std::string                                                 GetTransferEncoding() const;
        std::string                                                 GetRequestURI() const;
        std::string                                                 GetRequestLine() const;
        std::string                                                 GetHTTPVersion() const;
        std::string                                                 GetMethod() const;
        std::string                                                 GetContentType() const;
        std::string                                                 GetConnection() const;
        std::string                                                 GetQuery() const;
        std::string                                                 GetReasonPhrase() const;
        
        //Setters
        void                                                        SetTransferEncoding(std::string value);
        void                                                        SetRequestURI(std::string value);
        void                                                        SetRequestLine(std::string value);
        void                                                        SetHTTPVersion(std::string value);
        void                                                        SetMethod(std::string value);
        void                                                        SetConnection(std::string value);
        void                                                        SetQuery(std::string value);
        void                                                        SetReasonPhrase(std::string value);
        void                                                        SetStatusCode(int value);
        void                                                        SetContentLength(int value);
        void                                                        SetContentType(std::string value);
        void                                                        SetHeader(std::map<std::string, std::string> value);
        void                                                        SetBody(std::vector<std::pair<std::string, std::string> > value);
        void                                                        SetQueryStringParam(std::vector<std::pair<std::string, std::string> > value);

        
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
        std::vector<std::string>                                    SplitRequest(std::string data);
        std::string                                                 FillRequestLine();//change this and work with SearchLine();
        void                                                        SplitLine(std::string Line);
        void                                                        FillRequestURI();
        bool                                                        iSValidURI();
        void                                                        SpecialGetContentType(std::string   Value);
        void                                                        FillQuery();
        void                                                        FillQueryStringParam();
        void                                                        IsRequestFinished();

        //Functions For Testing Only
        void                                                        PrintVectorOfPairs(std::vector<std::pair<std::string, std::string> >    Body);
};

Request                                                            *FillLines(std::vector<std::string>    SingleRequest);
void                                                                PrintData(std::vector<std::vector<std::string> >  RequestData);
bool                                                                HandleRequest(std::string _readStr, int sd, std::map<int, Client *>	*ClientsInformation);

std::string                                                         LandingPage(std::string path);// autoindex

#endif
