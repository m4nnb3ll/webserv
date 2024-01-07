/* ************************************************************************** */
/*												                              */
/*												           :::      ::::::::  */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*												       +:+ +:+         +:+    */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*												   +#+#+#+#+#+   +#+          */
/*   Created: 2023/12/13 14:17:44 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/07 21:18:06 by abelayad         ###   ########.fr       */
/*												                              */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <ft_common.h>
#include "Client.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Config.hpp"

class	Config;
class	Location;
class	Response;
class   Client;

/* 

    Allowed Methods ( GET | POST | DELETE )

    The CRLF is the standard line ending sequence in HTTP.
    CRLF "\r\n" //Carriage Return && Line Feed
    LF "\n" // Line Feed
    CR "\r" // Carriage Return

*/


#define FILE_MAX_SIZE 10000000 //10MB (For Testing Only)

//struct t_request for Parsing the Request
typedef struct s_request
{
    std::vector<std::pair<std::string, std::string> >   		queryStringParam;
    std::vector<std::pair<std::string, std::string> >   		body;
    std::map<std::string, std::string>                  		header;
    std::string                                         		transferEncoding;
    std::string                                         		BoundaryBegin;
    std::string                                         		reasonPhrase;
    std::string                                         		requestLine;
    std::string                                         		contentType;
    std::string                                         		httpVersion;
    std::string                                         		HeaderInStr;
    std::string                                         		connection;
    std::string                                         		method;
    std::string                                         		query;
    std::string                                         		BodyInStr;
    std::string                                         		data;
    std::string                                         		host;
    std::string                                         		uri;
    std::string                                         		BoundaryEnd;
    size_t												 		contentLength;
    size_t												 		BodySize;
    bool												   		isFinished;
    bool												   		hasHeader;
    bool												   		hasBody;
    bool												   		Status;
    int												    		statusCode;
} t_request;

class Request
{
    public:
        Request(std::string);
        ~Request();
        Request(const Request &other);
        Request &operator=(const Request &other);

		void													handleRequest(int, Config*);
		//Getters
        std::vector<std::pair<std::string, std::string> >       getQueryStringParam() const;
        std::vector<std::pair<std::string, std::string> >       getBody() const;
        std::map<std::string, std::string>                      getHeader() const;
        std::string												getTransferEncoding() const;
        std::string												getRequestLine() const;
        std::string												getHTTPVersion() const;
        std::string												getMethod() const;
        std::string												getContentType() const;
        std::string												getConnection() const;
        std::string												getQuery() const;
        std::string												getReasonPhrase() const;
        std::string												getUri() const;
        std::string												getHost() const;
        size_t												    getContentLength() const;
        int												        getStatusCode() const;
        Location						                        *getLocation() const;
		Response*						                        getResponse() const;
        std::string/*will retrieve the content of the body*/    getContent() const;
        
        //Setters
		void														appendStr(std::string);
        void												       setQueryStringParam(std::vector<std::pair<std::string, std::string> > value);
        void												       setBody(std::vector<std::pair<std::string, std::string> > value);
        void												       setHeader(std::map<std::string, std::string> value);
        void												       setTransferEncoding(std::string value);
        void												       setReasonPhrase(std::string value);
        void												       setContentType(std::string value);
        void												       setRequestLine(std::string value);
        void												       setHTTPVersion(std::string value);
        void												       setConnection(std::string value);
        void												       setMethod(std::string value);
        void												       setQuery(std::string value);
        void												       setUri(std::string value);
        void												       setStatusCode(int value);
        void												       setContentLength(int value);
        void												       setHost(std::string value);
		void										            setResponse(Response *);
        void										            setLocation(int sd, Config* conf);
        
        void												       specialGetContentType(std::string   Value);
        void												      	CpDataFromStruct(t_request ReqParse);
        void													prepareResponse();
        void												       SearchErrors();


        //Functions For Testing Only
        void												       printVectorOfPairs(std::vector<std::pair<std::string, std::string> >    Body);

        //Variables
        std::vector<std::pair<std::string, std::string> >       files;
        std::vector<std::string>                                lines;
        std::string												boundaryBegin;
        std::string												boundaryEnd;
        std::string												headerInStr;
        std::string												body;
        size_t												     bodySize;
        bool												       isFinished;
        bool												       isHeader;
        bool												       status;
        bool												       isBody;

    
    private:
		std::string												_reqStr;
        std::vector<std::pair<std::string, std::string> >       _queryStringParam;
        std::vector<std::pair<std::string, std::string> >       _body;
        std::map<std::string, std::string>                      _header;
        std::string												_transferEncoding;
        std::string												_reasonPhrase;
        std::string												_requestLine;
        std::string												_httpVersion;
        std::string												_contentType;
        std::string												_connection;
        std::string												_method;
        std::string												_query;
        std::string												_host;
        std::string												_uri;
        Location												*_location;
		Response												*_response;
        size_t												     _contentLength;
        int												        _statusCode;

		void													_traitRequest(t_request ReqParse);
};

std::string												        LandingPage(std::string path);// autoindex
void												               setupHeader(t_request *Req, std::string Line);
void												               getHeaderAsPairVector(t_request *Req);
t_request												          isCompletedRequest(std::string data);
void												               getHeaderVariables(t_request *Req);
bool												               isTerminatedChunks(t_request *Req);
void												               obtainRequestLine(t_request *Req);
bool												               isCompletedHeader(t_request *Req);
void												               updateHeader(t_request *Req);
void												               isInvalidUri(t_request *Req);
void												               treatingBody(t_request *Req);
size_t												             getBodySize(t_request *Req);
void												               updateBody(t_request *Req);

//Functions For Testing Only
void												               PrintData(std::vector<std::vector<std::string> >  RequestData);

#endif
