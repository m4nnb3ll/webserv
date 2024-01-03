#include <ft_common.h>
#include "Config.hpp"
#include "Request.hpp"

std::map<std::string, std::string> initEnv(int sd, Location *serverLocation, std::map<int, Client *> ClientsInformation)
{
    std::cout << "\nim in initEnv fun";
    Client *client = ClientsInformation[sd];
    std::cout << "\nlocation path -> " << serverLocation->getAllowMethods()[0];
    std::cout << "\nget Content type -> " << client->_clientRequest->_getHeader()["Content-Type"];
    std::cout << "\nget query -> " << client->_clientRequest->_getQuery();
    std::map<std::string, std::string> env;
    std::stringstream oss;
    oss << client->_clientRequest->_bodySize;
    env["REDIRECT_STATUS"] = "200";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SCRIPT_NAME"] = serverLocation->getRootPath();     // take a virtual path /myapp/cgi/script.py
    env["SCRIPT_FILENAME"] = serverLocation->getRootPath(); // take a abolute path /var/www/html/myapp/cgi/script.py
    env["REQUEST_METHOD"] = client->_clientRequest->_getMethod();
    env["CONTENT_LENGTH"] = oss.str(); // take body
    env["CONTENT_TYPE"] = client->_clientRequest->_getHeader()["Content-Type"];  // --> header : Content Type
    env["PATH_INFO"] = client->_clientRequest->_getRequestURI(); //might need some change, using config path/contentLocation
    env["QUERY_STRING"] = client->_clientRequest->_getQuery();
    // env["REQUEST_URI"] = request.getPath() + request.getQuery(); // uri complet must add when morad push it project
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "Weebserv/1.0";
    return(env);
}

char					**getEnvCStyleArray(std::map<std::string, std::string> env) {
	char	**_env = new char*[env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i =  env.begin(); i !=  env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		_env[j] = new char[element.size() + 1];
		_env[j] = strcpy(_env[j], (const char*)element.c_str());
		j++;
	}
	_env[j] = NULL;
	return _env;
}

std::string cgi(std::string file, int sd, Location *serverLocation, std::map<int, Client *> ClientsInformation)
{

    std::cout << "\n->> inside CGI <<- \n";
    getEnvCStyleArray(initEnv(sd, serverLocation, ClientsInformation));// inital env
    int pipes[2];
    (void)file;
    if (pipe(pipes) < 0)
        perror("pipe failed");
    pid_t pid = fork();
    std::string buffer;
    if (pid == 0)
    {
        close(pipes[0]);
        dup2(pipes[1], 1);

        execlp("/usr/bin/php", "/usr/bin/php", file.c_str(), NULL);
        perror("failed to exec");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {

            close(pipes[1]);
            dup2(pipes[0], 0);
            char res[2048];

            int z = read(pipes[0], res, 2048);
            for (int i = 0; i < z; i++)
            {
                // std::cout << res[i];
                buffer += res[i];
            }

            close(pipes[0]);
            close(pipes[1]);
        }
        else
            buffer = "INTERNAL SERVER 500";
    }

    return (buffer);
}

// void sendResponseTest(std::string message, int sd, int isText)
// {
//     std::ostringstream http_response;
//     (void)isText;
//     http_response << "HTTP/1.1 200 OK\r\n";
//     http_response << "Content-Type: text/html\r\n";

//     std::string content(message);
//     http_response << "Content-Length: " << content.size();
//     http_response << "\r\n\r\n"
//                   << content;
//     send(sd, http_response.str().c_str(), http_response.str().size(), 0);
// }

// void Config::_sendResponse(int sd, std::map<int, Client *> ClientsInformation)
// {
//     ServersSocket *sS = _sdToServersSocket[sd];
//     Server *server = sS->getServers()[0];
//     std::vector<std::string> serverName = server->getServerNames();         //  servers
//     const std::vector<Location *> &serverLocation = server->getLocations(); // get location
//     Client *client = ClientsInformation[sd];
 
//     if (client && client->_clientRequest->_getMethod() == "GET" && client->_isSend != true)
//     {
//          std::ifstream file((serverLocation[0]->getRootPath() + client->_clientRequest->_getRequestURI()).c_str()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== OPEN FILE AND CHECK EXTENSION

//          for (size_t a = 0; a < serverLocation.size(); a++)
//         {
//             std::map<std::string, std::string> isCgi = serverLocation[a]->getCgi();
//             std::map<std::string, std::string>::iterator it = isCgi.begin();
//             std::ifstream file((serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI()).c_str()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== OPEN FILE AND CHECK EXTENSION
//             if (client->_clientRequest->_getRequestURI() == serverLocation[a]->getPath() && client->_isSend != true)    // if / == /
//             {                                                                                                        // handle if autoindex on or off , and check is file or not
//                 std::vector<std::string> path;
//                 std::string pathDir = serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI();
//                 DIR *dir = opendir(pathDir.c_str());

//                 path.push_back(client->_clientRequest->_getRequestURI());
//                 if ((dir || path.back() == "/") && client->_isSend != true) // if a directory
//                 {
//                     std::cout << "is a directory\n";
//                     if (serverLocation[a]->getAutoIndex()) // autoindex ON
//                     {

//                         sendResponseTest(LandingPage(serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI()), sd, 0);
//                         client->_isSend = true;
//                         break;
//                     }
//                     else if (client->_isSend != true) // check indexes
//                     {
//                         size_t count = 0;
//                         for (size_t i = 0; i < serverLocation[a]->getIndexes().size(); i++)
//                         {
//                             std::cout << "------------";
//                             std::ifstream file((serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]).c_str());
//                             std::cout << "search index = " << serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i] << "\n";
//                             // std::cout << "file found = " << file.good() << "\n";
//                             if (file.good()) // if found this index
//                             {
//                                 std::cout << "im here\n";
//                                 if (serverLocation[a]->getIndexes()[i].find(".php") != std::string::npos)
//                                 {
//                                     if (!isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php")
//                                     {
//                                         sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, serverLocation[a], ClientsInformation), sd, 0);
//                                         client->_isSend = true;
//                                         break;
//                                     }
//                                     else
//                                         sendResponseTest("internal server error 500", sd, 0);
//                                     break;
//                                 }
//                                 else
//                                 { // another extension
//                                     sendResponseTest(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, 1);
//                                     client->_isSend = true;
//                                     break;
//                                 }
//                             }
//                             else
//                                 count++;
//                         }
//                     }
//                 }
//             }
//             else if (file.good() && client->_isSend != true) // if file
//             {

//                 std::string path = serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI();
//                 std::cout << path;
//                 if (path.find(".php") != std::string::npos)
//                 {
//                     std::cout << ">>> IM2 HERE  <<<\n";
//                     sendResponseTest(cgi(path, sd, serverLocation[a], ClientsInformation), sd, 0);
//                     client->_isSend = true;
//                     break;
//                 }
//                 else if (serverLocation[a]->getAutoIndex()) // autoindex ON
//                 {
//                     sendResponseTest(LandingPage(serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI()), sd, 0);
//                     client->_isSend = true;
//                     break;
//                 }
//                 else // check indexes
//                 {
//                     size_t count = 0;
//                     for (size_t i = 0; i < serverLocation[a]->getIndexes().size(); i++)
//                     {
//                         std::ifstream file((serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]).c_str());
//                         std::cout << "search index = " << serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i] << "\n";
//                         // std::cout << "file found = " << file.good() << "\n";
//                         if (file.good()) // if found this index
//                         {
//                             std::cout << "im here\n";
//                             if (serverLocation[a]->getIndexes()[i].find(".php") != std::string::npos)
//                             {
//                                 if (!isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php")
//                                 {
//                                     sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, serverLocation[a], ClientsInformation), sd, 0);
//                                     client->_isSend = true;
//                                 }
//                                 else
//                                     sendResponseTest("internal server error 500", sd, 0);
//                                 break;
//                             }
//                             else
//                             { // another extension
//                                 sendResponseTest(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, 1);
//                                 client->_isSend = true;
//                                 break;
//                             }
//                         }
//                         else
//                             count++;
//                     }
//                     if (count == serverLocation[a]->getIndexes().size() && client->_isSend != true) // not found this indexes
//                         break;
//                 }
//             }
//         }
//         if (client && !client->_isSend)
//         {
//             sendResponseTest("<h1 style=\"color:red\">NOT FOUND 404</h1>", sd, 0);
//         }
//     }
// }