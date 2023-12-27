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
{
    // Method = "";
    StatusCode = -1;
    BodySize = 0;
    // ContentLength = 0;
    // TransferEncoding = "";
    // RequestLine = "";
    // HTTPVersion = "";
    // RequestURI = "";
    // ContentType = "";
    // Connection = "";
    // Query = "";
}

Request::~Request(){}

Request::Request(const Request &other){ (void)other; }

Request &Request::operator=(const Request &other)
{
    if (this != &other){}
    return (*this);
}

std::vector<std::pair<std::string, std::string> >   Request::GetQueryStringParam() const { return (QueryStringParam); }

std::vector<std::pair<std::string, std::string> >   Request::GetBody() const { return (Body); }

std::map<std::string, std::string>                  Request::GetHeader() const { return (Header); }

std::string                                         Request::GetRequestURI() const { return (RequestURI); }

std::string                                         Request::GetRequestLine() const { return (RequestLine); }

std::string                                         Request::GetHTTPVersion() const { return (HTTPVersion); }

std::string                                         Request::GetMethod() const { return (Method); }

std::string                                         Request::GetConnection() const { return (Connection); }

std::string                                         Request::GetQuery() const { return (Query); }

std::string                                         Request::GetContentType() const { return (ContentType); };

std::string                                         Request::GetTransferEncoding() const { return (TransferEncoding);  }

int                                                 Request::GetStatusCode() const { return (StatusCode); }

size_t                                              Request::GetContentLength() const { return (ContentLength); }

std::string                                         Request::GetReasonPhrase() const { return (ReasonPhrase); }


void                                                Request::SetTransferEncoding(std::string value) { TransferEncoding = value;  }

void                                                Request::SetQueryStringParam(std::vector<std::pair<std::string, std::string> > value) { QueryStringParam = value; }

void                                                Request::SetBody(std::vector<std::pair<std::string, std::string> > value){ Body = value; }

void                                                Request::SetRequestURI(std::string value) { RequestURI = value; }

void                                                Request::SetRequestLine(std::string value) { RequestLine= value; }

void                                                Request::SetHTTPVersion(std::string value) { HTTPVersion = value; }

void                                                Request::SetMethod(std::string value) { Method = value; }

void                                                Request::SetStatusCode(int value) { StatusCode = value; }

void                                                Request::SetHeader(std::map<std::string, std::string> value) { Header = value; }

void                                                Request::SetContentType(std::string value) { ContentType = value; }

void                                                Request::SetContentLength(int value) {ContentLength = value;}

void                                                Request::SetConnection(std::string value){ Connection = value; }

void                                                Request::SetQuery(std::string value){ Query = value; }

void                                                Request::SetReasonPhrase(std::string value) { ReasonPhrase = value; }

bool SearchLine(std::string Line, std::string Content)
{
    if (Line.find(Content) <= Line.size())
        return (true);
    return (false);
}

bool    Request::iSValidURI()
{
   std::string  uri_Characters;
   
   uri_Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
   for (size_t i = 0; i < RequestURI.size(); i++)
   {
       if (uri_Characters.find(RequestURI[i], 0) > uri_Characters.size())
       {
            //400 => Bad Request | Uploading a file that is too large | Invalid Cookies | DNS cache error
            StatusCode = 400;
            //fill ReasonPhrase any time you Find Error
            return (false);
       }
   }
   //check the size of URI (limit it in 2,048 characters!!!) | 414 URI Too Long
   if (RequestURI.size() > 2048)//I'm Not Sure About That
       StatusCode = 414;
   return (true);
}

void Request::SpecialGetContentType(std::string     Value)
{
    size_t begin = Value.find(";");
    for (size_t i = 0; i < begin && begin <= Value.size(); i++)
        ContentType += Value[i];
    begin = Value.find("=");
    boundary += "--";
    for (size_t i = begin + 1; i < Value.size() && begin <= Value.size(); i++)//skep (;Space)
    {
        if (Value[i] == '\n' || Value[i] == '\r')
            break ;
        boundary += Value[i];
    }
    boundary += "\r";
}

void Request::SplitLine(std::string Line)
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
            SpecialGetContentType(Value);
        else
            ContentType = Value;
    }
    if (SearchLine(Line, "Content-Length"))
        ContentLength = static_cast<int>(strtod(Value.c_str(), NULL));
    if (SearchLine(Line, "Connection"))
        Connection = Value;
    if (SearchLine(Line, "Transfer-Encoding"))//check this later
        TransferEncoding = Value;
    Header.insert(std::make_pair(Key, Value));
}

void Request::FillRequestURI()
{
    size_t          begin;
    size_t          end;

    begin = RequestLine.find(" ");
    if (begin >= RequestLine.size())
        return ;
    end = RequestLine.find(" ", begin + 1);
    if (end >= RequestLine.size())
        return ;
    for (size_t i = begin + 1; i < end; i++)
        RequestURI += RequestLine[i];
    for (size_t i = 0; i < begin; i++)
        Method += RequestLine[i];
    for (size_t i = end + 1; i < RequestLine.size(); i++)
        HTTPVersion += RequestLine[i];
    if (!iSValidURI())
        return ;//return error code
}

std::vector<std::string> Request::SplitRequest(std::string data)
{
    std::istringstream          s(data);
    std::vector<std::string>    Lines;
    std::string                 Line;
    int                         i;

    i = 0;
    while (std::getline(s, Line))//check for CRLF | CR | LF !
    {
        if (i > 0)
            SplitLine(Line);
        Lines.push_back(Line);
        i++;
    }
    return (Lines);
}

std::string  Request::FillRequestLine()//change this and work with SearchLine()
{
    //Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    if (Lines.size() > 0)
        RequestLine = *(Lines.begin());
    return (RequestLine);
}

void FormOne(Request *Request)
{
    bool                                                Status;
    std::string                                         Line;
    std::string                                         Key;
    std::string                                         Value;
    std::vector<std::pair<std::string, std::string> >   Body;
    std::istringstream                                  ss(Request->AllBody);

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
    Request->SetBody(Body);
}

std::vector<std::string> SplitBody(std::string data)
{
    std::istringstream          s(data);
    std::vector<std::string>    Lines;
    std::string                 Line;

    while (std::getline(s, Line))//check for CRLF | CR | LF
    {
        Lines.push_back(Line);
    }
    return (Lines);
}

void FormTwo(Request *Request)
{
    std::vector<std::pair<std::string, std::string> >   Body;
    std::vector<std::string>                            data; 
    std::string                                         value;
    std::string                                         key;

    data = SplitBody(Request->AllBody);
    for (std::vector<std::string>::iterator iter = data.begin(); iter != data.end(); iter++)
    {
        if (*iter == Request->boundary)
        {
            iter++;
            size_t begin = (*iter).find("=");
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
    Request->SetBody(Body);
}

void    ConvertBodyToKeyValue(Request *Request)
{
    if (Request->GetContentType() == "application/x-www-form-urlencoded")
        FormOne(Request);
    else if (Request->GetContentType() == "multipart/form-data")
        FormTwo(Request);
}

void FillBody(Request *Request, std::string data)
{
    size_t begin;
    
    begin = data.find("\r\n\r\n");
    if (begin > data.size())
        return;
    for (size_t i = begin; i < data.size(); i++)
    {
        if ((data[i] == '\r' || data[i] == '\n') && Request->AllBody == "")
            continue;
        Request->BodySize++;
        Request->AllBody += data[i];
    }
    // std::cout << Request->AllBody << std::endl;
    ConvertBodyToKeyValue(Request);
    // Request->PrintVectorOfPairs(Request->GetBody());
}

void    Request::FillQueryStringParam()
{
    std::istringstream  s(Query);
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
        QueryStringParam.push_back(std::make_pair(key, value));
        Status = false;
        key = "";
        value = "";
    }
    // PrintVectorOfPairs(QueryStringParam);
}

void Request::FillQuery()
{
    size_t begin;

    begin = RequestURI.find("?");
    if (begin <= RequestURI.size())
    {
        for(size_t i = begin + 1; i < RequestURI.size(); i++)
            Query += RequestURI[i];
    }
    FillQueryStringParam();
}

void Request::IsRequestFinished()
{
    if (Method == "POST" && ContentType.empty() && TransferEncoding.empty())
        StatusCode = 400;//Bad Request
    if (Method != "POST" && Method != "GET" && Method != "DELETE")
        StatusCode = 405;// Method Not Allowed | check it later
    if (Method == "POST" && BodySize == ContentLength && StatusCode == -1)
        isFinished = true;
    else
        isFinished = false;
}

Request *    FillLines(std::string    SingleRequest)
{
    Request                     *Req;

    Req = new Request();
    Req->Lines = Req->SplitRequest(SingleRequest);
    Req->FillRequestLine();
    Req->FillRequestURI();
    Req->FillQuery();
    if (Req->GetMethod() == "POST")
        FillBody(Req, SingleRequest);
    Req->IsRequestFinished();
    return (Req);
}

void	HandleRequest(std::string _readStr, int sd, std::map<int, Client *>	*ClientsInformation)
{
	Request				                *Req;
    Client                              *Clt;
    std::map<int, Client *>::iterator   iter;
    
    Clt = new Client();
	Req = new Request();
	Req = FillLines(_readStr);
	Clt->ClientRequest = Req;
    iter = ClientsInformation->find(sd);
    if (iter != ClientsInformation->end())
        iter->second = Clt;//update this line if you work with vector of Requests! | leaks here
    else
	    ClientsInformation->insert(std::make_pair(sd, Clt));
	// PrintMap(ClientsInformation);
}

void    Request::PrintVectorOfPairs(std::vector<std::pair<std::string, std::string> >           Body)
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