#include "Cgi.hpp"

Cgi::Cgi(Request *request, Location *config)
{
    // this->_body = request->_getBody(); 
    std::cout << "cgi\n"; 

    this->_initEnv(request, config);
}

Cgi::~Cgi() {}

void Cgi::_initEnv(Request *request, Location *config)
{
    (void)config;
    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
    }
    std::cout << "cwd" << std::string(cwd)  + request->_getRequestURI();
    std::stringstream oss;
    oss << request->_bodySize;
    _env["REDIRECT_STATUS"] = "200"; // some cgi script may check this variable to use this status
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["SCRIPT_NAME"] = request->_getRequestURI(); // take a virtual path /myapp/cgi/script.py
    _env["SCRIPT_FILENAME"] = std::string(cwd) + request->_getRequestURI();            // path of file.php
    _env["REQUEST_METHOD"] = request->_getMethod();
    _env["CONTENT_LENGTH"] = oss.str().size();                    // take body
    _env["CONTENT_TYPE"] = request->_getHeader()["Content-Type"]; // --> header : Content Type
    _env["PATH_INFO"] = request->_getRequestURI();
    _env["QUERY_STRING"] = request->_getQuery();
    _env["REQUEST_URI"] = request->_getRequestURI() + request->_getQuery(); // uri complet must add when morad push it project
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

std::string		Cgi::executeCgi(std::string file) {
	pid_t		pid;
	char		**env;
	std::string	newBody;
    (void)file;
    int pipes[2];
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