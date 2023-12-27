/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:17:44 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/23 18:39:25 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Request_HPP
#define Request_HPP

//change the name of file to HttpRequest or the reverse
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
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
        std::vector<std::pair<std::string, std::string> >           Body;
        std::vector<std::pair<std::string, std::string> >           QueryStringParam;
        std::map<std::string, std::string>                          Header;
        std::string                                                 Method;
        size_t                                                      ContentLength;
        int                                                         StatusCode;
        std::string                                                 ReasonPhrase;
        std::string                                                 TransferEncoding;
        std::string                                                 RequestLine;
        std::string                                                 HTTPVersion;
        std::string                                                 RequestURI;
        std::string                                                 ContentType;
        std::string                                                 Connection;
        std::string                                                 Query;
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

        
        std::vector<std::string>                                    Lines;
        std::string                                                 AllBody;
        size_t                                                      BodySize;
        std::string                                                 boundary;
        bool                                                        isFinished;

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
void                                                                HandleRequest(std::string _readStr, int sd, std::map<int, Client *>	*ClientsInformation);

#endif