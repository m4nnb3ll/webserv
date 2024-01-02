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

Request::Request(const std::string reqStr)
	:	_headerComplete(false),
		_bodyComplete(false),
		_statusCode(-1),
		_bodySize(0),
		_contentLength(0),
		_isFinished(false),
		_status(false),
		_reqStr(reqStr) {}

Request::~Request(){}

void	Request::appendStr(const std::string reqStr) { _reqStr += reqStr; }

std::vector<std::pair<std::string, std::string> >   Request::getQueryStringParam() const { return (_queryStringParam); }

std::vector<std::pair<std::string, std::string> >   Request::getBody() const { return (_body); }

std::map<std::string, std::string>                  Request::getHeader() const { return (_header); }

std::string                                         Request::getRequestURI() const { return (_requestURI); }

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


void                                                Request::setTransferEncoding(std::string value) { _transferEncoding = value;  }

void                                                Request::setQueryStringParam(std::vector<std::pair<std::string, std::string> > value) {_queryStringParam = value; }

void                                                Request::setBody(std::vector<std::pair<std::string, std::string> > value){ _body = value; }

void                                                Request::setRequestURI(std::string value) { _requestURI = value; }

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

bool SearchLine(std::string Line, std::string Content)
{
    if (Line.find(Content) <= Line.size())
        return (true);
    return (false);
}

bool    Request::isValidURI()
{
   std::string  uri_Characters;
   
   uri_Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
   for (size_t i = 0; i < _requestURI.size(); i++)
   {
       if (uri_Characters.find(_requestURI[i], 0) > uri_Characters.size())
       {
            //400 => Bad Request | Uploading a file that is too large | Invalid Cookies | DNS cache error
            _statusCode = 400;
            _reasonPhrase = "Bad Request";
            return (false);
       }
   }
   //check the size of URI (limit it in 2,048 characters!!!) | 414 URI Too Long
   if (_requestURI.size() > 2048)//I'm Not Sure About That
   {
       _statusCode = 414;
       _reasonPhrase = "URI Too Long";
   }
   return (true);
}

void Request::specialGetContentType(std::string     Value)
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

void Request::splitLine(std::string Line)
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
            specialGetContentType(Value);
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
    _header.insert(std::make_pair(Key, Value));
}

void Request::fillRequestURI()
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
        _requestURI += _requestLine[i];
    for (size_t i = 0; i < begin; i++)
        _method += _requestLine[i];
    for (size_t i = end + 1; i < _requestLine.size(); i++)
        _httpVersion += _requestLine[i];
    if (!_isValidURI())
        return ;//return error code
}

std::vector<std::string>	Request::splitRequest(std::string data)
{
    std::istringstream          s(data);
    std::vector<std::string>    Lines;
    std::string                 Line;
    int                         i;

    i = 0;
    while (std::getline(s, Line))//check for CRLF | CR | LF !
    {
        if (i > 0)
            splitLine(Line);
        Lines.push_back(Line);
        i++;
    }
    return (Lines);
}

std::string  Request::fillRequestLine()//change this and work with SearchLine()
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
    Req->setBody(Body);
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
                return (true);
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

// void FillAllHeader(std::string data, Request *Req)
// {
//     size_t begin;

//     begin = data.find(D_CRLF);
//     if (begin > data.size())
//         return;
//     for (size_t i = 0; i < begin; i++)
//         Req->_allHeader += data[i];
// }

void FillBody(Request *Req, std::string data)
{
    size_t begin;
    
    begin = data.find(D_CRLF);
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

void    Request::fillQueryStringParam()
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

void Request::fillQuery()
{
    size_t          begin;
    size_t          UriEnd;
    std::string     Uri;

    begin = _requestURI.find("?");
    if (begin > _requestURI.size())
        UriEnd = _requestURI.size();
    else
        UriEnd = begin;
    for(size_t i = 0; i < UriEnd; i++)
        Uri += _requestURI[i];
    if (begin <= _requestURI.size())
    {
        for(size_t i = begin + 1; i < _requestURI.size(); i++)
            _query += _requestURI[i];
    }
    _fillQueryStringParam();
    _requestURI = Uri;
}

void Request::isRequestFinished()
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

Request     *parseRequest(std::string reqStr)
{
    Request                     *Req;

    Req = new Request();
    Req->_lines = Req->splitRequest(reqStr);
    Req->fillRequestLine();
    Req->fillRequestURI();
    Req->fillQuery();
    FillAllHeader(reqStr, Req);
    if (Req->getMethod() == "POST")
        FillBody(Req, reqStr);
    if (Req->_Files.size() > 0 && Req->getStatusCode() == -1)
        CreateFiles(Req);
    Req->isRequestFinished();
    return (Req);
}

bool    HandleRequest(std::string reqStr, int sd, std::map<int, Client *>	*sdToClient)
{
    Client                              *client;
    std::map<int, Client *>::iterator   iter;
    
    if (!request->_isFinished)
        return (false);
    client = (*sdToClient)[sd];
    if (client)
        client->request = parseRequest(reqStr);
    else
    {
        client = new Client();
        client -> request = parseRequest(reqStr);
        sdToClient->insert(std::make_pair(sd, client));
    }
    return (true);
}

void    Request::_printVectorOfPairs(std::vector<std::pair<std::string, std::string> >	Body)
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

Request*	Request::_updateHeader()
{
	if (!_headerComplete)
	{
		size_t crlfPos = _reqStr.find(D_CRLF);
		if (crlfPos != std::npos)
		{
			_header = _reqStr.substr(0, crlfPos);
			_prepareHeader();
			_headerComplete = true;
		}
	}
	return (this);
}

Request*	Request::_updateBody()
{
	if (!_bodyComplete)
	{

	}
	return (this);
}

void	_fillHeaderField()
{
	// this function will check if the header line is valid or not
}

void	_prepareHeader()
{
	std::istringstream	iSS(_header);
	std::string			line;

    while (std::getline(iSS, line))
		_fillHeaderField(line);
}

void	Request::update()
{
	this
	._updateHeader()
	._updateBody();
}