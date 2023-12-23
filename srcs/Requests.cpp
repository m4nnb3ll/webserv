/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyahy <mbouyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:45:11 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/22 12:43:53 by mbouyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Requests.hpp"

Requests::Requests(){}

Requests::~Requests(){}

Requests::Requests(const Requests &other){ (void)other; }

Requests &Requests::operator=(const Requests &other)
{
    if (this != &other){}
    return (*this);
}

std::vector<std::pair<std::string, std::string> >   Requests::GetBody() const { return (Body); }

std::map<std::string, std::string>                  Requests::GetHeader() const { return (Header); }

std::string                                         Requests::GetRequestURI() const { return (RequestURI); }

std::string                                         Requests::GetRequestLine() const { return (RequestLine); }

std::string                                         Requests::GetHTTPVersion() const { return (HTTPVersion); }

std::string                                         Requests::GetMethod() const { return (Method); }

std::string                                         Requests::GetConnection() const { return (Connection); }

std::string                                         Requests::GetQuery() const { return (Query); }

std::string                                         Requests::GetContentType() const { return (ContentType); };

int                                                 Requests::GetErrorCode() const { return (ErrorCode); }

int                                                 Requests::GetContentLength() const { return (ContentLength); }


void                                                Requests::SetBody(std::vector<std::pair<std::string, std::string> > value){ Body = value; }

void                                                Requests::SetRequestURI(std::string value) { RequestURI = value; }

void                                                Requests::SetRequestLine(std::string value) { RequestLine= value; }

void                                                Requests::SetHTTPVersion(std::string value) { HTTPVersion = value; }

void                                                Requests::SetMethod(std::string value) { Method = value; }

void                                                Requests::SetErrorCode(int value) { ErrorCode = value; }

void                                                Requests::SetHeader(std::map<std::string, std::string> value) { Header = value; }

void                                                Requests::SetContentType(std::string value) { ContentType = value; }

void                                                Requests::SetContentLength(int value) {ContentLength = value;}

void                                                Requests::SetConnection(std::string value){ Connection = value; }

void                                                Requests::SetQuery(std::string value){ Query = value; }

bool SearchLine(std::string Line, std::string Content)
{
    if (Line.find(Content) <= Line.size())
        return (true);
    return (false);
}

bool    Requests::iSValidURI()
{
   std::string  uri_Characters;
   
   uri_Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
   for (size_t i = 0; i < RequestURI.size(); i++)
   {
       if (uri_Characters.find(RequestURI[i], 0) > uri_Characters.size())
       {
            //400 => Bad Request | Uploading a file that is too large | Invalid Cookies | DNS cache error
            ErrorCode = 400;
            return (false);
       }
   }
   //check the size of URI (limit it in 2,048 characters!!!) | 414 URI Too Long
   if (RequestURI.size() > 2048)//I'm Not Sure About That
       ErrorCode = 414;
   return (true);
}

void Requests::SpecialGetContentType(std::string     Value)
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

void Requests::SplitLine(std::string Line)
{
    size_t          begin;
    std::string     Value;
    std::string     Key;

    if (Line.empty())
        return ;
    begin = Line.find(":");
    if (begin >= Line.size())
    {
        Requests::ErrorCode = 1;
        return ;
    }
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
    Header.insert(std::make_pair(Key, Value));
}

void Requests::FillRequestURI()
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

std::vector<std::string> Requests::SplitRequest(std::string data)
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

std::string  Requests::FillRequestLine()//change this and work with SearchLine()
{
    //Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    if (Lines.size() > 0)
        RequestLine = *(Lines.begin());
    return (RequestLine);
}

void FormOne(Requests *Request)
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

void FormTwo(Requests *Request)
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

void    ConvertBodyToKeyValue(Requests *Request)
{
    if (Request->GetContentType() == "application/x-www-form-urlencoded")
        FormOne(Request);
    else if (Request->GetContentType() == "multipart/form-data")
        FormTwo(Request);
}

void FillBody(Requests *Request, std::string data)
{
    size_t begin;
    
    begin = data.find("\r\n\r\n");
    if (begin > data.size())
        return;
    for (size_t i = begin; i < data.size(); i++)
    {
        if ((data[i] == '\r' || data[i] == '\n') && Request->AllBody == "")
            continue;
        Request->AllBody += data[i];
    }
    ConvertBodyToKeyValue(Request);
    // Request->PrintVectorOfPairs(Request->GetBody());
}

void Requests::FillQuery()
{
    size_t begin;

    begin = RequestURI.find("?");
    if (begin <= RequestURI.size())
    {
        for(size_t i = begin + 1; i < RequestURI.size(); i++)
            Query += RequestURI[i];
    }
}

Requests *    FillLines(std::vector<std::string>    SingleRequest)
{
    Requests *Request = new Requests();
    
    for (std::vector<std::string>::iterator SimpleIter = SingleRequest.begin(); \
            SimpleIter != SingleRequest.end(); SimpleIter++)//i will remove this later
    {
        Request->Lines = Request->SplitRequest(*SimpleIter);
        Request->FillRequestLine();
        Request->FillRequestURI();
        Request->FillQuery();
        if (Request->GetMethod() == "POST")
            FillBody(Request, *SimpleIter);
    }
    return (Request);
}

void	HandleRequest(std::string _readStr, int sd, std::map<int, Client *>	*ClientsInformation, Client *_Client)
{
	Requests				*Request;
	std::vector<std::string>	SingleRequest;

	Request = new Requests();
	SingleRequest.push_back(_readStr);
	Request = FillLines(SingleRequest);
	_Client->ClientRequest = Request;
	ClientsInformation->insert(std::make_pair(sd, _Client));
	// PrintMap(ClientsInformation);
}

void    Requests::PrintVectorOfPairs(std::vector<std::pair<std::string, std::string> >           Body)
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