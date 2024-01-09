#include "Cgi.hpp"

Cgi::Cgi(Response* response)
{
    // this->_body = request->_getBody(); 
    std::cout << "cgi\n"; 

    this->_initEnv(response->getRequest(), response);
}

Cgi::~Cgi() {}

void Cgi::_initEnv(Request *request, Response *response)
{
    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
    }
    std::cout << "cwd" << std::string(cwd)  + request->getUri();
    std::cout << RED << ">>" << std::string(cwd) + "/" + response->getResource() << "<<" << RESET_COLOR << std::endl;
	// std::stringstream oss;
    // oss << request->bodySize;
    _env["REDIRECT_STATUS"] = "200"; // some cgi script may check this variable to use this status
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["SCRIPT_NAME"] = std::string(cwd) + "/" + response->getResource(); // take a virtual path /myapp/cgi/script.py
    _env["SCRIPT_FILENAME"] = std::string(cwd) + "/" + response->getResource();            // path of file.php
    _env["REQUEST_METHOD"] = request->getMethod();
    _env["CONTENT_LENGTH"] = request->getContentLength();                    // take body
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

std::string		Cgi::execute()
{
	pid_t		pid;
	char		**env;
	std::string	newBody;
    int 		pipes[2];

    pipe(pipes);
	try {
		env = this->_getEnvAsCstrArray();
	}
	catch (std::bad_alloc &e) {
		std::cerr  << e.what() << std::endl;
	}

	 
	int		ret = 1;
 

	pid = fork();

	if (pid == -1)
	{
		std::cerr   << "Fork crashed." << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		char * const * nll = NULL;
 
        close(pipes[0]);
            dup2(pipes[1], 1);
		execve("./cgi-bin/php-cgi", nll, env);
		std::cerr  << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[BUFFER_CGI] = {0};

		waitpid(-1, NULL, 0);
		close(pipes[1]);
            dup2(pipes[0], 0);
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

	if (!pid)
		exit(0);



	return (newBody);
}
