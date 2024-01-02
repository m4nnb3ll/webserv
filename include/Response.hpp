#pargma once

class Response
{
    public:
        // Setters
        setStatusCode();
        appendStr();
        // Getters
        getStatusCode();//Stoppedd here
        getStr();
    private:
        int         _statusCode;
        std::string _str;
}

void		setStatusCode(int code) { this->_statusCode = code; };
void		appendStr(std::string str) { this->_str = str; };
int			getStatusCode() { return (_statusCode); };
std::string	getStr() const { return (_str) };