/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:45:11 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/23 18:38:51 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string buff)
	:	_reqStr(buff), _location(NULL)
{}

Request::~Request(){}

Request::Request(const Request &other){ (void)other; }

Request &Request::operator=(const Request &other)
{
    if (this != &other){}
    return (*this);
}

void	Request::appendStr(std::string buff) { _reqStr += buff; }

/*
the role of FormOne Function :
    1- if the content type is application/x-www-form-urlencoded => spliting the body and store it in a vector of pairs
*/
void FormOne(Request *Req)
{
    bool                                                Status;
    std::string                                         Line;
    std::string                                         Key;
    std::string                                         Value;
    std::vector<std::pair<std::string, std::string> >   Body;
    std::istringstream                                  ss(Req->body);

    while (getline(ss, Line, '&'))
    {
        std::istringstream s(Line);
        Status = false;
        Value = "";
        Key = "";
        while (getline(s, Line, '='))
        {
            Status ? Value = Line : Key = Line;
            Status = true;
        }
        Body.push_back(std::make_pair(Key, Value));
    }
    Req->setBody(Body);
}

/*
the role of SplitBody Function :
    1- spliting the body and store it in a vector of strings
*/
std::vector<std::string> SplitBody(std::string data, Request *Req)
{
    std::vector<std::string>    Lines;
    std::string                 Line;
    size_t                      begin;

    begin = data.find("\r\n");
    while (begin != std::string::npos)
    {
        Line = data.substr(0, begin);
        Lines.push_back(Line);
        if (Line == Req->boundaryEnd)
            break ;
        data.erase(0, begin + 2);
        Line.clear();
        begin = data.find("\r\n");
    }
    return (Lines);
}

/*
the role of isFile Function :
    1- check if the content of the body is a file or not
*/
bool    isFile(std::vector<std::string>::iterator  iter, std::vector<std::string>   data)
{
    size_t begin;
    
    begin = (*iter).find(" filename=");
    if (begin < (*iter).size())
    {
        iter++;
        if (iter != data.end())
        {
            begin = (*iter).find("Content-Type: ");
            if (begin < (*iter).size())
            {
                return (true);
            }
        }
    }
    return (false);
}

/*
the role of HandleFiles Function :
    1- get the name and the content of the file
    2- store the name and the content of the file in a vector of pairs
*/
std::vector<std::string>::iterator    HandleFiles(Request *Req, std::vector<std::string>::iterator  iter, std::vector<std::string>   data)
{
    size_t                                              begin;
    std::string                                         value;
    std::string                                         key;
 
    begin = (*iter).find(" filename=");
    if (begin != std::string::npos)
    {
        for (size_t i = begin + 11; i < (*iter).size(); i++)
        {
            if (i + 1 < (*iter).size() && ((*iter)[i + 1] == '\r' || (*iter)[i + 1] == '\n'))
                break ;
            key += (*iter)[i];
        }
        if (iter + 2 != data.end())
        {
            iter+=2;
            for (; iter != data.end(); iter++)
            {
                if (*iter == "\n" || *iter == "\r")
                    continue;
                if (*iter == Req->boundaryEnd || *iter == Req->boundaryBegin)
                {
                    Req->isFinished = true;
                    break ;
                }
                    value += *iter + '\n';
            }
        }
    }
    Req->files.push_back(std::make_pair(key, value));
    return (iter);
}

/*
the role of FormTwo Function :
    1- spliting and check if the content of body is completed , if it's store it in a vector of pairs
    2- if a file found => call HandleFiles Function
    3- store the content of the body in a vector of pairs Like (Key | Value)
*/
void FormTwo(Request *Req)
{
    std::vector<std::pair<std::string, std::string> >   Body;
    std::vector<std::string>                            data; 
    std::string                                         value;
    std::string                                         key;

    data = SplitBody(Req->body, Req);
    for (std::vector<std::string>::iterator iter = data.begin(); iter != data.end(); iter++)
    {
        if (*iter == Req->boundaryBegin)
        {
            iter++;
            if (isFile(iter, data))
            {
                iter = HandleFiles(Req, iter, data);//handle files
                if (*iter == Req->boundaryBegin)
                    iter--;
            }
            else
            {
                size_t begin = (*iter).find("=");//change from "=" to " name="
                for (size_t i = begin + 2; i < (*iter).size() && begin <= (*iter).size(); i++)
                {
                    if (i + 1 < (*iter).size() && ((*iter)[i + 1] == '\r' || (*iter)[i + 1] == '\n'))
                        break ;
                    key += (*iter)[i];
                }
                //
                // if (begin != std::string::npos)
                //     key = (*iter).substr(begin + 7, (*iter).size() - begin - 2);//" name=" => 7
                if (iter + 2 != data.begin())
                {
                    iter += 2;
                    value = *iter;
                }
                Body.push_back(std::make_pair(key, value));
                key = "";
                value = "";
            }
        }
        if (*iter == Req->boundaryEnd)
            break ;
    }
    Req->setBody(Body);
}

/*
the role of IsHexNumber Function :
    1- check if the number is HexNumber or not
*/
bool    IsHexNumber(std::string Number)
{
    std::string HexNumber = "0123456789abcdefABCDEF";
    for (size_t i = 0; i < Number.size(); i++)
    {
        if (HexNumber.find(Number[i]) > HexNumber.size())
            return (false);
    }
    return (true);
}

/*
the role of GetLineByIndex Function :
    1- return the content of the stock from start to end
*/
std::string GetLineByIndex(std::string stock, size_t start, size_t end)
{
    std::string Line;

    for (size_t i = start; i < end; i++)
        Line += stock[i];
    return (Line);
}

/*
the role of SearchErrors Function :
    1- Check Errors
*/
void Request::SearchErrors()//Temp Function
{
    //check this later
    if (_method == "POST" && _contentType.empty() && _transferEncoding.empty())
        _statusCode = 400;//Bad Request
    if (_method != "POST" && _method != "GET" && _method != "DELETE")
        _statusCode = 405;// _method Not Allowed | check it later
    if (_method == "POST" && bodySize == _contentLength && _statusCode == -1)
        isFinished = true;
    else if (_method == "GET" && _statusCode == -1)
        isFinished = true;
    else if (_method == "DELETE" && _statusCode == -1)
        isFinished = true;
}

/*
the role of CpDataFromStruct Function :
    1- transfer the data from tReq to Req
*/
void Request::CpDataFromStruct(t_request ReqParse)
{
    _queryStringParam 	= ReqParse.queryStringParam;
    _transferEncoding 	= ReqParse.transferEncoding;
    _contentLength		= ReqParse.contentLength;
    _reasonPhrase 		= ReqParse.reasonPhrase;
    _contentType 		= ReqParse.contentType;
    _requestLine 		= ReqParse.requestLine;
    _connection 		= ReqParse.connection;
    _httpVersion 		= ReqParse.httpVersion;
    _statusCode 		= ReqParse.statusCode;
    _header 			= ReqParse.header;
    _method 			= ReqParse.method;
    _query 				= ReqParse.query;
    _body 				= ReqParse.body;
    _host 				= ReqParse.host;
    _uri 				= ReqParse.uri;
    boundaryBegin 		= ReqParse.BoundaryBegin;
    boundaryEnd 		= ReqParse.BoundaryEnd;
    headerInStr 		= ReqParse.HeaderInStr;
    isFinished 		    = ReqParse.isFinished;
    isHeader 			= ReqParse.hasHeader;
    bodySize 			= ReqParse.BodySize;
    isBody 			    = ReqParse.hasBody;
    status				= ReqParse.Status;
    body 				= ReqParse.BodyInStr;
}

/*
the role of traitRequest Function :
    1- create a new Request
    2- transfer the data from ReqParse to Req
    3- check if the ContentType is multipart/form-data, if it's call FormTwo Function
    4- check if the ContentType is application/x-www-form-urlencoded , if it's call FormOne Function
    5- call SearchErrors Function
    6- return Req
*/
void	Request::_traitRequest(t_request ReqParse)
{
    this->CpDataFromStruct(ReqParse);
    if (this->getContentType() == "multipart/form-data")
        FormTwo(this);
    else if (this->getContentType() == "application/x-www-form-urlencoded")
        FormOne(this);
    this->SearchErrors();
}

/*
the role of isCompletedHeader Function :
    1- check if the header is completed or not
    2- if the header is completed update the content of pHeader and return true
*/
bool    isCompletedHeader(t_request *ReqParse)
{
    size_t begin;

    begin = ReqParse->data.find("\r\n\r\n");
    if (begin != std::string::npos)
    {
        std::string str = ReqParse->data.substr(0, begin + 2);//i add 2 HERE FOR RESOLVE THE PROBLEM OF THE END OF THE HEADER!!!!
        ReqParse->HeaderInStr += str;
        // pRequest.erase(0, begin + 4);
        return (true);
    }
    return (false);
}

/*
the role of setupHeader Function :
    1- get the content of every Line of Header and store it the Key and the Value
    2- store the Key and the Value in _header vector of pairs
*/
void setupHeader(t_request *ReqParse, std::string Line)
{
    size_t          begin;
    std::string     Value;
    std::string     Key;

    if (Line.empty())
        return ;
    begin = Line.find(":");
    if (begin == std::string::npos)
        return ;
    Key = Line.substr(0, begin);
    Value = Line.substr(begin + 2, Line.size());//skep (:Space)
    ReqParse->header.insert(std::make_pair(Key, Value));
}

void   getHeaderAsPairVector(t_request *ReqParse)
{
    std::string Line;
    std::string temp;

    size_t begin;
    temp = ReqParse->HeaderInStr;
    begin = temp.find("\r\n");
    while (begin != std::string::npos)
    {
        Line = temp.substr(0, begin);
        temp.erase(0, begin + 2);
        setupHeader(ReqParse, Line);
        begin = temp.find("\r\n");
    }
}

/*
the role of specialGetContentType Function :
    1- get the ContentType
    2- get the BoundaryBegin
    3- get the BoundaryEnd
*/
void specialGetContentType(t_request *ReqParse, std::string Value)
{
    //change the structure of the function
    size_t begin;
    begin = Value.find(";");
    if (begin != std::string::npos)
        ReqParse->contentType = Value.substr(0, begin);
    begin = Value.find("=");
    if (begin != std::string::npos)
    {
        ReqParse->BoundaryBegin += "--";
        ReqParse->BoundaryBegin += Value.substr(begin + 1, Value.size());
        ReqParse->BoundaryEnd = ReqParse->BoundaryBegin + "--";
    }
}

/*
the role of takeQueryStringParam Function :
    1- get the QueryStringParam from the QueryString
    2- store the QueryStringParam in _queryStringParam vector of pairs
*/
void    takeQueryStringParam(t_request *ReqParse)
{
    std::istringstream  s(ReqParse->query);
    std::string         line;
    std::string         key;
    std::string         value;
    bool                Status;

    while (getline(s, line, '&'))
    {
        std::istringstream ss(line);
        while (getline(ss, line, '='))
        {
            Status ? value = line :key = line;
            Status = true;
        }
        ReqParse->queryStringParam.push_back(std::make_pair(key, value));
        Status = false;
        key = "";
        value = "";
    }
}

/*
the role of getQueryString Function :
    1- get the URI from the header
    2- get the QueryString from the URI
    3- store the QueryString in _query
    4- update the URI in _uri
*/
void getQueryString(t_request *ReqParse)
{
    size_t          begin;
    size_t          UriEnd;
    std::string     Uri;

    //change the structure of the function
    begin = ReqParse->uri.find("?");
    if (begin > ReqParse->uri.size())
        UriEnd = ReqParse->uri.size();
    else
        UriEnd = begin;
    for(size_t i = 0; i < UriEnd; i++)//check it later
        Uri += ReqParse->uri[i];
    if (begin <= ReqParse->uri.size())
    {
        for(size_t i = begin + 1; i < ReqParse->uri.size(); i++)//check it later
            ReqParse->query += ReqParse->uri[i];
    }
    takeQueryStringParam(ReqParse);
    ReqParse->uri = Uri;
}

/*
the role of getHeaderVariables Function :
    1- get the values of (Content-Length, Connection, Transfer-Encoding, 
        Content-Type, _method, _uri, _httpVersion, QueryString) from the header and store it in variables
*/
void    getHeaderVariables(t_request *ReqParse)
{
    if (ReqParse->header.empty())
        return ;
    if (ReqParse->header["Content-Length"] != "")
        ReqParse->contentLength = static_cast<int>(strtod(ReqParse->header["Content-Length"].c_str(), NULL));
    if (ReqParse->header["Connection"] != "")
        ReqParse->connection = ReqParse->header["Connection"];
    if (ReqParse->header["Host"] != "")
        ReqParse->host = ReqParse->header["Host"];
    if (ReqParse->header["Transfer-Encoding"] != "")//check this later
        ReqParse->transferEncoding = ReqParse->header["Transfer-Encoding"];
    if (ReqParse->header["Content-Type"] != "")
    {
        if (ReqParse->header["Content-Type"].find("multipart/form-data") <= ReqParse->header["Content-Type"].size())
        {
            specialGetContentType(ReqParse, ReqParse->header["Content-Type"]);
            ReqParse->Status = true;
        }
        if (!ReqParse->Status)
            ReqParse->contentType = ReqParse->header["Content-Type"];
    }
    std::istringstream  ss(ReqParse->requestLine);
    ss >> ReqParse->method >> ReqParse->uri >> ReqParse->httpVersion;
    getQueryString(ReqParse);
}

/*
the role of obtainRequestLine Function :
    1- obtain the request line from the header
*/

void obtainRequestLine(t_request *ReqParse)
{
    size_t      begin;

    begin = ReqParse->HeaderInStr.find("\r\n");
    if (begin != std::string::npos)
    {
        std::string str = ReqParse->HeaderInStr.substr(0, begin);
        ReqParse->requestLine += str;
        ReqParse->HeaderInStr.erase(0, begin + 2);//check it later
    }
}

/*
the role of isSpecialCharacter Function :
    1- check if the character is a special character or not
*/
bool isSpecialCharacter(const char c)
{
    std::string SpecialCharacters;

    SpecialCharacters = "-._~:/?#[]@!$&'()*+,;=%";
    for (size_t i = 0; i < SpecialCharacters.size(); i++)
    {
        if (c == SpecialCharacters[i])
            return (true);
    }
    return (false);
}

/*
the role of isInvalidUri Function :
    1- check if the URI is too long or not
    2- check if the URI contains invalid characters or not
    3- if (1 || 2 is true) => set the value of pIsFinished to true and store the error code in _statusCode and the error message in _reasonPhrase
*/
void    isInvalidUri(t_request *ReqParse)
{
    if (ReqParse->uri.size() > 2048)//I'm Not Sure About That
    {
        ReqParse->statusCode = 414;
        ReqParse->reasonPhrase = "URI Too Long";
        ReqParse->isFinished = true;
    }
    for (size_t i = 0; i < ReqParse->uri.size(); i++)
    {
        if (!isalpha(ReqParse->uri[i]) && !isdigit(ReqParse->uri[i]) && !isSpecialCharacter(ReqParse->uri[i]))
        {
            ReqParse->statusCode = 400;//=> Bad Request | Uploading a file that is too large | Invalid Cookies | DNS cache error
            ReqParse->reasonPhrase = "Bad Request";
            ReqParse->isFinished = true;
        }
    }
}

/*
the role of initializeRequest Function :
    1- initialize the content of Req
*/
void initializeRequest(t_request  *ReqParse, std::string data)
{
    ReqParse->data 				= data;
    ReqParse->isFinished 		= false;
    ReqParse->hasBody 			= false;
    ReqParse->hasHeader 		= false;
    ReqParse->Status 			= false;
    ReqParse->BodyInStr  		= "";
    ReqParse->HeaderInStr 		= "";
    ReqParse->contentType 		= "";
    ReqParse->connection 		= "";
    ReqParse->transferEncoding 	= "";
    ReqParse->reasonPhrase 		= "";
    ReqParse->method 			= "";
    ReqParse->BoundaryBegin 	= "";
    ReqParse->BoundaryEnd 		= "";
    ReqParse->requestLine 		= "";
    ReqParse->uri 				= "";
    ReqParse->httpVersion 		= "";
    ReqParse->query 			= "";
    ReqParse->BodySize 			= 0;
    ReqParse->contentLength 	= 0;
    ReqParse->statusCode 		= -1;
    ReqParse->queryStringParam.clear();
    ReqParse->body.clear();
    ReqParse->header.clear();
}

/*
the role of updateHeader Function :
    1- check if the header is completed
    2- if the header is completed update the content of pHeader and storing the content of it in variables
    3- check if the method is GET or DELETE => set the value of pIsFinished to true
    4- if ( 2 is true ) set the value of pIsHeader to true
*/
void updateHeader(t_request *ReqParse)
{
    if (!ReqParse->hasHeader)
    {
        if (isCompletedHeader(ReqParse))
        {
            obtainRequestLine(ReqParse);
            getHeaderAsPairVector(ReqParse);
            getHeaderVariables(ReqParse);
            isInvalidUri(ReqParse);//if found error => pIsFinished = true;
            if (ReqParse->method == "GET" || ReqParse->method == "DELETE")
                ReqParse->isFinished = true;
            ReqParse->hasHeader = true;
        }
    }
}

/*
the role of getBodySize Function :
    1- set the value of pBodySize to the size of pBody
    1- return the size of pBody
*/
size_t getBodySize(t_request *ReqParse)
{
    ReqParse->BodySize = ReqParse->BodyInStr.size();
    return (ReqParse->BodySize);
}

/*
the role of isTerminatedChunks Function :
    1- check if the Body is completed or not
    2- if the body is completed update the content of pBody and set the value of pIsBody to true
*/
bool    isTerminatedChunks(t_request *ReqParse)
{
    std::string     Stock;
    std::string     body;
    std::string     Line;
    size_t          end;

    Stock = ReqParse->BodyInStr;
    end = Stock.find("\r\n");
    end+=2;
    while (end != Stock.size())
    {
        Line = GetLineByIndex(Stock, 0, end - 2);
        if (!IsHexNumber(Line) && ReqParse->contentType == "application/x-www-form-urlencoded")
        {
            if (!body.empty())
                body += '&';
            body += GetLineByIndex(Stock, 0, end-2);
        }
        else if (!IsHexNumber(Line))
            body += GetLineByIndex(Stock, 0, end);
        else if (Line == "0")
        {
            ReqParse->hasBody = true;
            ReqParse->BodyInStr = body;
            return (true);
        }
        Stock.erase(0, end);
        end = Stock.find("\r\n");
        end+=2;
    }
    return (false);
}

/*
the role of treatingBody Function :
   1- Treat the body and and update the content of pBody
   2- if the body is completed set pIsBody to true
*/
void    treatingBody(t_request *ReqParse)
{
    size_t begin;
    
    begin = ReqParse->data.find("\r\n\r\n");
    if (begin == std::string::npos)
        return ;
    ReqParse->BodyInStr = ReqParse->data.substr(begin + 4, ReqParse->data.size());
    if (ReqParse->contentLength != 0)
    {
        if (getBodySize(ReqParse) == ReqParse->contentLength)
            ReqParse->hasBody = true;
    }
    else if (ReqParse->contentLength == 0 && ReqParse->transferEncoding == "chunked")
    {
        if (isTerminatedChunks(ReqParse))
            ReqParse->hasBody = true;
    }
    else if (ReqParse->contentLength == 0 && ReqParse->transferEncoding == "")
    {
        ReqParse->statusCode = 411;//check it later
        ReqParse->reasonPhrase = "Length Required";
        ReqParse->isFinished = true;
    }
}

/*
the role of updateBody Function : 
    1- check if the header is completed
    2- if the body is completed or not
    3- if the request is completed or not
    if (1 && 2 && 3) => call treatingBody Function
*/
void updateBody(t_request  *ReqParse)
{
    if (!ReqParse->hasBody && ReqParse->hasHeader && !ReqParse->isFinished)
    {
        treatingBody(ReqParse);
    }
}

/*
the role of isCompletedRequest Function : 
    check if the request is completed or not
*/
t_request isCompletedRequest(std::string data)
{
    t_request  ReqParse;

    initializeRequest(&ReqParse, data);
    updateHeader(&ReqParse);
    updateBody(&ReqParse);
    if (ReqParse.hasHeader && ReqParse.hasBody)
        ReqParse.isFinished = true;
    return (ReqParse);
}

/*
the role of HandleRequest Function : 
    0- parse the request and store it in tReq
    1- check if the request is completed or not
    2- if the request is completed => create a new Client and add it to the map
    3- if the request is not completed => return false
*/
// MAKE SURE TO MAKE THIS PART OF THE Request class @mourad
void	Request::handleRequest(int sd, Config* conf)
{
    t_request               ReqParse;

    ReqParse = isCompletedRequest(_reqStr);//rename this variable
    if (!ReqParse.isFinished) return ;
    _traitRequest(ReqParse);
    this->setLocation(sd, conf);
	// PrintMap(conf->getSdToClient());
    // To see later what to return
}

void	Request::setLocation(int sd, Config* conf)
{
	std::vector<Location*>	locations;
	size_t					length = 0;
	std::string				path;

	locations = conf->getServersSocket(sd)->getServer(_host)->getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		path = locations[i]->getPath();
		if (path.size() > _uri.size()) continue ;
		size_t	tmpLength = 0;
		for (size_t j = 0; j < _uri.size() && path[j] == _uri[j]; j++)
			tmpLength++;
		if (tmpLength > length)
		{
			length = tmpLength;
			_location = locations[i];
		}
	}
}

Location	*Request::getLocation() const
{
	return (_location);
}

std::string/*will retrieve the content of the body*/		Request::getContent() const
{
	// for test only
	return ("SIMPLE TEXT MESSAGE\n");
}

/*
    Getters
*/
std::vector<std::pair<std::string, std::string> >   Request::getQueryStringParam() const { return (_queryStringParam); }
std::vector<std::pair<std::string, std::string> >   Request::getBody() const { return (_body); }
std::map<std::string, std::string>                  Request::getHeader() const { return (_header); }
std::string                                         Request::getUri() const { return (_uri); }
std::string                                         Request::getRequestLine() const { return (_requestLine); }
std::string                                         Request::getHTTPVersion() const { return (_httpVersion); }
std::string                                         Request::getMethod() const { return (_method); }
std::string                                         Request::getConnection() const { return (_connection); }
std::string                                         Request::getQuery() const { return (_query); }
std::string                                         Request::getContentType() const { return (_contentType); };
std::string                                         Request::getTransferEncoding() const { return (_transferEncoding);  }
int                                                 Request::getStatusCode() const { return (_statusCode); }
size_t                                              Request::getContentLength() const { return (_contentLength); }
std::string                                         Request::getReasonPhrase() const { return (_reasonPhrase); }
std::string                                         Request::getHost() const { return (_host); }
Response*											Request::getResponse() const { return (_response); }

/*
    Setters
*/
void                                                Request::setTransferEncoding(std::string value) { _transferEncoding = value;  }
void                                                Request::setQueryStringParam(std::vector<std::pair<std::string, std::string> > value) {_queryStringParam = value; }
void                                                Request::setBody(std::vector<std::pair<std::string, std::string> > value){ _body = value; }
void                                                Request::setUri(std::string value) { _uri = value; }
void                                                Request::setRequestLine(std::string value) { _requestLine= value; }
void                                                Request::setHTTPVersion(std::string value) { _httpVersion = value; }
void                                                Request::setMethod(std::string value) { _method = value; }
void                                                Request::setStatusCode(int value) { _statusCode = value; }
void                                                Request::setHeader(std::map<std::string, std::string> value) { _header = value; }
void                                                Request::setContentType(std::string value) { _contentType = value; }
void                                                Request::setContentLength(int value) {_contentLength = value;}
void                                                Request::setConnection(std::string value){ _connection = value; }
void                                                Request::setQuery(std::string value){ _query = value; }
void                                                Request::setReasonPhrase(std::string value) { _reasonPhrase = value; }
void                                                Request::setHost(std::string value){ _host = value; }
void												Request::setResponse(Response *response) { _response = response; }

//Test Functions
void    Request::printVectorOfPairs(std::vector<std::pair<std::string, std::string> >           Body)
{
    std::vector<std::pair<std::string, std::string> >::iterator           iter;
    for (iter = Body.begin(); iter != Body.end(); iter++)
        std::cout << "Key : " << iter->first << " | Value : " << iter->second << std::endl;
}

void PrintData(std::vector<std::vector<std::string> >  RequestData)
{
    for (std::vector<std::vector<std::string> >::iterator iter = RequestData.begin() ; iter != RequestData.end(); iter++)
        for (std::vector<std::string>::iterator SimpleIter = iter->begin() ; SimpleIter != iter->end(); SimpleIter++)
            std::cout << "* : " << *SimpleIter << std::endl;
}
