#include "Cgi.hpp"

Cgi::Cgi(Response* response, std::string cgiPath, std::string filePath)
	:	_cgiPath(cgiPath), _filePath(filePath), _response(response)
{
	this->_initEnv();
}

Cgi::~Cgi() {}

void Cgi::_initEnv()
{
	char cwd[100];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("getcwd");
	}
	Request	*request = _response->getRequest();
	// std::cout << "cwd" << std::string(cwd)  + request->getUri();
	// std::cout << RED << ">>" << std::string(cwd) + "/" + _response->getResource() << "<<" << RESET_COLOR << std::endl;
	// std::stringstream oss;
	// oss << request->bodySize;
	_env["REDIRECT_STATUS"] = "200"; // some cgi script may check this variable to use this status
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SCRIPT_NAME"] = _filePath; // take a virtual path /myapp/cgi/script.py
	_env["SCRIPT_FILENAME"] = _filePath; // path of file.php
	_env["REQUEST_METHOD"] = request->getMethod();
	_env["CONTENT_LENGTH"] = request->getContentLength();					// take body
	_env["CONTENT_TYPE"] = request->getHeader()["Content-Type"]; // --> header : Content Type
	_env["PATH_INFO"] = request->getUri();
	_env["QUERY_STRING"] = request->getQuery();
	_env["REQUEST_URI"] = request->getUri() + request->getQuery(); // uri complet must add when morad push it project
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "Weebserv/1.0";
	std::cout << "\n end  \n";
}

char **Cgi::_getEnvAsCstrArray() const
{
	char **env = new char *[_env.size() + 1];
	int j = 0;
	for (std::map<std::string, std::string>::const_iterator i = _env.begin(); i != _env.end(); i++)
	{
		std::string element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char *)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

std::string getFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos) {
        return filename.substr(dotPos + 1);
    }
    return ""; // If no extension found
}

std::string	Cgi::_addContentLength(const std::string& httpResponse)
{
	if (_filePath.substr(_filePath.find_last_of('.') + 1) != "php") return (httpResponse);
    
	size_t headerEndPos = httpResponse.find("\r\n\r\n");
    // Extract headers and body
    std::string headers = httpResponse.substr(0, headerEndPos);
    std::string body = httpResponse.substr(headerEndPos + 4); // Skip "\r\n\r\n"

    // Split headers into lines
    std::istringstream headersStream(headers);
    std::vector<std::string> headerLines;
    std::string line;
    while (std::getline(headersStream, line)) {
        if (!line.empty()) {
            headerLines.push_back(line);
        }
    }

    // Add the additional header
    std::ostringstream oSS;
    oSS << "Content-Length: " << body.size();
    headerLines.push_back(oSS.str());

    // Reconstruct headers
    std::ostringstream modifiedHeaders;
    for (size_t i = 0; i < headerLines.size(); ++i) {
        modifiedHeaders << headerLines[i];
        if (i < headerLines.size() - 1) {
            modifiedHeaders << "\r\n"; // Correctly use CRLF for line endings
        }
    }

    // Reconstruct the modified response
    return modifiedHeaders.str() + "\r\n\r\n" + body;
}

std::string	Cgi::execute()
{
	pid_t		pid;
	char		**env;
	std::string	newBody;
	int 		pipes[2];
	int			ret;

	pipe(pipes);
	try {
		env = this->_getEnvAsCstrArray();
	}
	catch (std::bad_alloc &e) {
		std::cerr  << e.what() << std::endl;
	}	 

	pid = fork();

	if (pid == -1)
		return ("\0");
	else if (!pid)
	{
		char * const * nll = NULL;
 
		close(pipes[0]);
			dup2(pipes[1], 1);
		// std::cout << "_cgiPath is: " << _cgiPath << std::endl;
		execve(_cgiPath.c_str(), nll, env);
		exit(1);
	}
	else
	{
		char	buffer[BUFFER_CGI] = {0};
		int		status;
		waitpid(-1, &status, 0);
		close(pipes[1]);
		dup2(pipes[0], 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return ("\0");
		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, BUFFER_CGI);
			ret = read(pipes[0], buffer, BUFFER_CGI - 1);
			newBody += buffer;
		}
	}
 
	close(pipes[1]);
	close(pipes[0]);

	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
	return ("HTTP/1.1 200 OK\r\n" + _addContentLength(newBody));
}
