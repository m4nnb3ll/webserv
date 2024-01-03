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

Request::Request()
	:	_location(NULL)
{
    _statusCode = -1;
    _bodySize = 0;
    _contentLength = 0;
    _isFinished = false;
    _status = false;
    // TransferEncoding = "";
    // RequestLine = "";
    // HTTPVersion = "";
    // ContentType = "";
    // RequestURI = "";
    // Connection = "";
    // Method = "";
    // Query = "";
}

Request::~Request(){}

Request::Request(const Request &other){ (void)other; }

Request &Request::operator=(const Request &other)
{
    if (this != &other){}
    return (*this);
}

std::vector<std::pair<std::string, std::string> >   Request::_getQueryStringParam() const { return (_queryStringParam); }

std::vector<std::pair<std::string, std::string> >   Request::_getBody() const { return (_body); }

std::map<std::string, std::string>                  Request::_getHeader() const { return (_header); }

std::string                                         Request::_getRequestURI() const { return (_uri); }

std::string                                         Request::_getRequestLine() const { return (_requestLine); }

std::string                                         Request::_getHTTPVersion() const { return (_httpVersion); }

std::string                                         Request::getHost() const { return (_host); }

std::string                                         Request::_getMethod() const { return (_method); }

std::string                                         Request::_getConnection() const { return (_connection); }

std::string                                         Request::_getQuery() const { return (_query); }

std::string                                         Request::_getContentType() const { return (_contentType); };

std::string                                         Request::_getTransferEncoding() const { return (_transferEncoding);  }

int                                                 Request::_getStatusCode() const { return (_statusCode); }

size_t                                              Request::_getContentLength() const { return (_contentLength); }

std::string                                         Request::_getReasonPhrase() const { return (_reasonPhrase); }


void                                                Request::_setTransferEncoding(std::string value) { _transferEncoding = value;  }

void                                                Request::_setQueryStringParam(std::vector<std::pair<std::string, std::string> > value) {_queryStringParam = value; }

void                                                Request::_setBody(std::vector<std::pair<std::string, std::string> > value){ _body = value; }

void                                                Request::_setRequestURI(std::string value) { _uri = value; }

void                                                Request::_setRequestLine(std::string value) { _requestLine= value; }

void                                                Request::_setHTTPVersion(std::string value) { _httpVersion = value; }

void                                                Request::_setMethod(std::string value) { _method = value; }

void                                                Request::_setStatusCode(int value) { _statusCode = value; }

void                                                Request::_setHeader(std::map<std::string, std::string> value) { _header = value; }

void                                                Request::_setContentType(std::string value) { _contentType = value; }

void                                                Request::_setContentLength(int value) {_contentLength = value;}

void                                                Request::_setConnection(std::string value){ _connection = value; }

void                                                Request::_setQuery(std::string value){ _query = value; }

void                                                Request::_setReasonPhrase(std::string value) { _reasonPhrase = value; }

bool SearchLine(std::string Line, std::string Content)
{
    if (Line.find(Content) <= Line.size())
        return (true);
    return (false);
}

bool    Request::_isValidURI()
{
   std::string  uri_Characters;
   
   uri_Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
   for (size_t i = 0; i < _uri.size(); i++)
   {
       if (uri_Characters.find(_uri[i], 0) > uri_Characters.size())
       {
            //400 => Bad Request | Uploading a file that is too large | Invalid Cookies | DNS cache error
            _statusCode = 400;
            _reasonPhrase = "Bad Request";
            return (false);
       }
   }
   //check the size of URI (limit it in 2,048 characters!!!) | 414 URI Too Long
   if (_uri.size() > 2048)//I'm Not Sure About That
   {
       _statusCode = 414;
       _reasonPhrase = "URI Too Long";
   }
   return (true);
}

void Request::_specialGetContentType(std::string     Value)
{
    size_t begin = Value.find(";");
    for (size_t i = 0; i < begin && begin <= Value.size(); i++)
        _contentType += Value[i];
    begin = Value.find("=");
    _boundaryBegin += "--";
    for (size_t i = begin + 1; i < Value.size() && begin <= Value.size(); i++)//skep (;Space)
    {
        if (Value[i] == '\n' || Value[i] == '\r')
            break ;
        _boundaryBegin += Value[i];
    }
    _boundaryEnd = _boundaryBegin + "--\r";
    _boundaryBegin += "\r";
}

void Request::_splitLine(std::string Line)
{
    size_t          begin;
    std::string     Value;
    std::string     Key;

    if (Line.empty())
        return ;
    begin = Line.find(":");
    if (begin >= Line.size())
        return ;
    for (size_t i = 0; i < begin; i++)
        Key += Line[i];
    for (size_t i = begin + 2; i < Line.size(); i++)//skep (:Space)
    {
        if (Line[i] == '\n' || Line[i] == '\r')
            break ;
        Value += Line[i];
    }
    if (SearchLine(Line, "Content-Type"))
    {
        if (Value.find("multipart/form-data") <= Value.size())
        {
            _specialGetContentType(Value);
            _status = true;
        }
        if (!_status)
            _contentType = Value;
        // else
        //     _statusCode = 415;//Unsupported Media Type
    }
    if (SearchLine(Line, "Content-Length"))
        _contentLength = static_cast<int>(strtod(Value.c_str(), NULL));
    if (SearchLine(Line, "Connection"))
        _connection = Value;
    if (SearchLine(Line, "Transfer-Encoding"))//check this later
        _transferEncoding = Value;
    if (SearchLine(Line, "Host"))//check this later
        _host = Value;
    _header.insert(std::make_pair(Key, Value));
}

void Request::_fillRequestURI()
{
    size_t          begin;
    size_t          end;

    begin = _requestLine.find(" ");
    if (begin >= _requestLine.size())
        return ;
    end = _requestLine.find(" ", begin + 1);
    if (end >= _requestLine.size())
        return ;
    for (size_t i = begin + 1; i < end; i++)
        _uri += _requestLine[i];
    for (size_t i = 0; i < begin; i++)
        _method += _requestLine[i];
    for (size_t i = end + 1; i < _requestLine.size(); i++)
        _httpVersion += _requestLine[i];
    if (!_isValidURI())
        return ;//return error code
}

std::vector<std::string> Request::_splitRequest(std::string data)
{
    std::istringstream          s(data);
    std::vector<std::string>    Lines;
    std::string                 Line;
    int                         i;

    i = 0;
    while (std::getline(s, Line))//check for CRLF | CR | LF !
    {
        if (i > 0)
            _splitLine(Line);
        Lines.push_back(Line);
        i++;
    }
    return (Lines);
}

std::string  Request::_fillRequestLine()//change this and work with SearchLine()
{
    //Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    if (_lines.size() > 0)
        _requestLine = *(_lines.begin());
    return (_requestLine);
}

void FormOne(Request *Req)
{
    bool                                                Status;
    std::string                                         Line;
    std::string                                         Key;
    std::string                                         Value;
    std::vector<std::pair<std::string, std::string> >   Body;
    std::istringstream                                  ss(Req->_allBody);

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
    Req->_setBody(Body);
}

std::vector<std::string> SplitBody(std::string data, Request *Req)
{
    (void)Req;
    std::istringstream          s(data);
    std::vector<std::string>    Lines;
    std::string                 Line;

    while (std::getline(s, Line))//check for CRLF | CR | LF
    {
        Lines.push_back(Line);
        if (Line == Req->_boundaryEnd)
        {
            Req->_isFinished = true;
            break ;
        }
    }
    return (Lines);
}

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

std::vector<std::string>::iterator    HandleFiles(Request *Req, std::vector<std::string>::iterator  iter, std::vector<std::string>   data)
{
    (void)iter;
    (void)data;
    (void)Req;
    size_t                                              begin;
    std::string                                         value;
    std::string                                         key;
 
    begin = (*iter).find(" filename=");
    if (begin < (*iter).size())
    {
        for (size_t i = begin + 11; i < (*iter).size() && begin <= (*iter).size(); i++)
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
                if (*iter == Req->_boundaryEnd || *iter == Req->_boundaryBegin)
                {
                    Req->_isFinished = true;
                    break ;
                }
                    value += *iter + '\n';
            }
        }
    }
    Req->_Files.push_back(std::make_pair(key, value));
    return (iter);
}

void FormTwo(Request *Req)
{
    std::vector<std::pair<std::string, std::string> >   Body;
    std::vector<std::string>                            data; 
    std::string                                         value;
    std::string                                         key;

    data = SplitBody(Req->_allBody, Req);
    for (std::vector<std::string>::iterator iter = data.begin(); iter != data.end() && (Req->_isFinished == true); iter++)
    {
        if (*iter == Req->_boundaryBegin)
        {
            iter++;
            if (isFile(iter, data))
            {
                iter = HandleFiles(Req, iter, data);//handle files
                if (*iter == Req->_boundaryBegin)
                    iter--;
            }
            else //*iter != Req->_boundaryEnd && *iter != Req->_boundaryBegin
            {
                size_t begin = (*iter).find("=");//change from "=" to " name="
                for (size_t i = begin + 2; i < (*iter).size() && begin <= (*iter).size(); i++)
                {
                    if (i + 1 < (*iter).size() && ((*iter)[i + 1] == '\r' || (*iter)[i + 1] == '\n'))
                        break ;
                    key += (*iter)[i];
                }
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
        if (*iter == Req->_boundaryEnd)
        {
            Req->_isFinished = true;
            break ;
        }
    }
    Req->_setBody(Body);
}

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

std::string GetLineByIndex(std::string stock, size_t start, size_t end)
{
    std::string Line;

    for (size_t i = start; i < end; i++)
        Line += stock[i];
    return (Line);
}

void    OrganizeBody(Request *Req)
{
    std::string     Stock;
    std::string     body;
    std::string     Line;
    size_t          end;

    Stock = Req->_allBody;
    end = Stock.find("\r\n");
    end+=2;
    while (end != Stock.size())
    {
        Line = GetLineByIndex(Stock, 0, end - 2);
        if (!IsHexNumber(Line) && Req->_getContentType() == "application/x-www-form-urlencoded")
        {
            if (!body.empty())
                body += '&';
            body += GetLineByIndex(Stock, 0, end-2);
        }
        else if (!IsHexNumber(Line))
            body += GetLineByIndex(Stock, 0, end);
        else if (Line == "0")
        {
            Req->_isFinished = true;
            break ;
        }
        Stock.erase(0, end);
        end = Stock.find("\r\n");
        end+=2;
    }
    Req->_allBody = body;
}

void    ConvertBodyToKeyValue(Request *Req)
{
    if (Req->_getTransferEncoding() == "chunked")
        OrganizeBody(Req);
    if (Req->_getContentType() == "multipart/form-data")
        FormTwo(Req);
    else if (Req->_getContentType() == "application/x-www-form-urlencoded")
        FormOne(Req);
}

void FillAllHeader(std::string data, Request *Req)
{
    size_t begin;

    begin = data.find("\r\n\r\n");
    if (begin > data.size())
        return;
    for (size_t i = 0; i < begin; i++)
        Req->_allHeader += data[i];
}

void FillBody(Request *Req, std::string data)
{
    size_t begin;
    
    begin = data.find("\r\n\r\n");
    if (begin > data.size())
        return;
    for (size_t i = begin; i < data.size(); i++)
    {
        if ((data[i] == '\r' || data[i] == '\n') && Req->_allBody == "")
            continue;
        Req->_bodySize++;
        Req->_allBody += data[i];
    }
    ConvertBodyToKeyValue(Req);
}

void    Request::_fillQueryStringParam()
{
    std::istringstream  s(_query);
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
        _queryStringParam.push_back(std::make_pair(key, value));
        Status = false;
        key = "";
        value = "";
    }
}

void Request::_fillQuery()
{
    size_t          begin;
    size_t          UriEnd;
    std::string     Uri;

    begin = _uri.find("?");
    if (begin > _uri.size())
        UriEnd = _uri.size();
    else
        UriEnd = begin;
    for(size_t i = 0; i < UriEnd; i++)
        Uri += _uri[i];
    if (begin <= _uri.size())
    {
        for(size_t i = begin + 1; i < _uri.size(); i++)
            _query += _uri[i];
    }
    _fillQueryStringParam();
    _uri = Uri;
}

void Request::_isRequestFinished()
{
    if (_method == "POST" && _contentType.empty() && _transferEncoding.empty())
        _statusCode = 400;//Bad Request
    if (_method != "POST" && _method != "GET" && _method != "DELETE")
        _statusCode = 405;// _method Not Allowed | check it later
    if (_method == "POST" && _bodySize == _contentLength && _statusCode == -1)
        _isFinished = true;
    else if (_method == "GET" && _statusCode == -1)
        _isFinished = true;
    else if (_method == "DELETE" && _statusCode == -1)
        _isFinished = true;
}

void    RemoveCreatedFiles(Request *Req, size_t i)
{
    std::string path;

    for (size_t j = 0; j < i; j++)
    {
        path = "./assets/" + Req->_Files[j].first;
        remove(path.c_str());
        path.clear();
    }
}

void CreateFiles(Request *Req)
{
    std::string    path;

    for (size_t i = 0; i < Req->_Files.size(); i++)
    {
        std::ofstream   file;
        // std::ifstream   ExistFile;
        path = "./assets/" + Req->_Files[i].first;
        file.open(path.c_str());
        // ExistFile.open(path.c_str());
        // if (ExistFile.is_open())//check if file is already exist
        if (!file.is_open())//check if file is opened
        {
            std::cout << "Error : " << strerror(errno) << std::endl;
            if (i > 0)
                RemoveCreatedFiles(Req, i);//i choice to work with atomic operation - it either fully completes or does not make any changes at all.
            Req->_setStatusCode(500);
            Req->_setReasonPhrase("Internal Server Error");//HTTP 500 Internal Server Error status code
            return ;//return error code
        }
        else if (Req->_Files[i].second.size() > FILE_MAX_SIZE)//check if file is too large
        {
            /*if the issue is that the client has uploaded a file that's too large,
                you might want to return a 413 Payload Too Large status code instead*/
            Req->_setStatusCode(413);
            Req->_setReasonPhrase("Payload Too Large");// HTTP 413 Payload Too Large status code
            file.close();
            return ;//return error code
        }
        //check if file is not empty!!!!!
        file << Req->_Files[i].second;
        file.close();
        // ExistFile.close();
        path.clear();
    }
}

Request     *FillLines(std::string    SingleRequest)
{
    Request                     *Req;

    Req = new Request();
    Req->_lines = Req->_splitRequest(SingleRequest);
    Req->_fillRequestLine();
    Req->_fillRequestURI();
    Req->_fillQuery();
    FillAllHeader(SingleRequest, Req);
    if (Req->_getMethod() == "POST")
        FillBody(Req, SingleRequest);
    if (Req->_Files.size() > 0 && Req->_getStatusCode() == -1)
        CreateFiles(Req);
    Req->_isRequestFinished();
    return (Req);
}

bool    HandleRequest(std::string _readStr, int sd, Config* conf)
{
	Request				                *Req;
    Client                              *Clt;
    std::map<int, Client *>::iterator   iter;
	std::map<int, Client*> sdToClient = conf->getSdToClient();
    
    Clt = new Client();
	Req = FillLines(_readStr);
    if (!Req->_isFinished)
    {
        delete Req;
        delete Clt;
        return (false);
    }
	Clt->_clientRequest = Req;
    iter = sdToClient.find(sd);
    if (iter != sdToClient.end())
        iter->second = Clt;//update this line if you work with vector of Requests! | leaks here
    else
	    sdToClient.insert(std::make_pair(sd, Clt));
	Req->setLocation(sd, conf);
	Req->prepareResponse();
	return (true);
}

void	_checkLocation()
{
	if (!_response.isFinished())
	{
		if (!_location)
			_response.setFinished(STATUS_NOT_FOUND);
		else if (_location -> getRedirect())
			_response.setFinished(STATUS_MOVED);
	}
}

void	_checkFinalMsg()
{
	std::ostringstream	oSS;
	if (_response.getContent().empty())
	{
		oSS << "HTTP/1.1" << " " << _response.getStatusCode() << " " << "OK" << "\r\n";
		_finalMsg = oSS.str();
	}
}

// Stopped here
void	Request::prepareResponse()
{
	_response = new Response();
	_checkLocation()
	_checkFinalMsg();
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

void    Request::_printVectorOfPairs(std::vector<std::pair<std::string, std::string> >           Body)
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
