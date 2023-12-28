#include "Config.hpp"
#include "Request.hpp"
#include <dirent.h>

std::string cgi(std::string file)
{
    std::cout << "\nCGI\n";
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
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {

        close(pipes[1]);
        dup2(pipes[0], 0);
        char res[2048];

        int z = read(pipes[0], res, 2048);
        for (int i = 0; i < z; i++)
        {
            std::cout << res[i];
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

void sendResponseTest(std::string message, int sd, int isHtml)
{
    std::ostringstream http_response;

    http_response << "HTTP/1.1 200 OK\r\n";
    http_response << "Content-Type: text/html\r\n";
    if (isHtml == 0)
    {

        std::string content("<html><body><h1>" + message + "</h1></body></html>");
        http_response << "Content-Length: " << content.size();
        http_response << "\r\n\r\n"
                      << content;

        send(sd, http_response.str().c_str(), http_response.str().size(), 0);
    }
    else
    {

        std::cout << "in send funtion \n";
        char buffer[2048];
        std::string res;
        std::string path = "/Users/asekkak/Desktop/webserv/wordpress/" + message;
        if (message.find("/") != std::string::npos)
            path = "/Users/asekkak/Desktop/webserv/wordpress" + message;
        int fd = open(path.c_str(), O_RDONLY);
        std::cout << fd;
        size_t z = read(fd, buffer, 2048);
        for (size_t i = 0; i < z; i++)
        {
            res += buffer[i];
        }

        std::string content("<html><body><h1>" + res + "</h1></body></html>");
        http_response << "Content-Length: " << content.size();
        http_response << "\r\n\r\n"
                      << content;

        send(sd, http_response.str().c_str(), http_response.str().size(), 0);
    }
}

void Config::_sendResponse(int sd, std::map<int, Client *> ClientsInformation)
{
    // To keep things simple, we are not going to use serverNames for now
    ServersSocket *sS = _sdToServersSocket[sd];
    Server *server = sS->getServers()[0];
    std::vector<std::string> serverName = server->getServerNames();         // name server
    const std::vector<Location *> &serverLocation = server->getLocations(); // name server
    // std::cout << serverLocation.size();
    Client *client = ClientsInformation[sd];
    // std::cout << client->ClientRequest->GetHeader()["Connection"];

    if (client->ClientRequest->GetMethod() == "GET")
    {
        std::ifstream file(serverLocation[0]->getRootPath() + client->ClientRequest->GetRequestURI()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== OPEN FILE AND CHECK EXTENSION
        std::cout << "root : " << serverLocation[0]->getRootPath() + client->ClientRequest->GetRequestURI() << "\n";
        std::cout << "file found :" << file.good() << "\n";
        for (size_t a = 0; a < serverLocation.size(); a++)
        {

            if (client->ClientRequest->GetRequestURI() == serverLocation[a]->getPath()) // if / == /
            {                                                                           // handle if autoindex on or off , and check is file or not
                std::vector<std::string> path;
                std::string pathDir = serverLocation[0]->getRootPath() + client->ClientRequest->GetRequestURI();
                DIR *dir = opendir(pathDir.c_str());

                path.push_back(client->ClientRequest->GetRequestURI());
                if (dir || path.back() == "/") // if a directory
                {
                    std::cout << "is a directory\n";
                    if (serverLocation[a]->getAutoIndex()) // autoindex ON
                    {
                        sendResponseTest(LandingPage(serverLocation[0]->getRootPath() + client->ClientRequest->GetRequestURI()), sd, 0);
                        client->isSend = true;
                        break;
                    }
                    else // check indexes
                    {
                        size_t count = 0;
                        for (size_t i = 0; i < serverLocation[a]->getIndexes().size(); i++)
                        {
                            std::ifstream file(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]);
                            std::cout << "search index = " << serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i] << "\n";
                            std::cout << "file found = " << file.good() << "\n";
                            if (file.good()) // if found this index
                            {
                                std::cout << "im here\n";
                                if (serverLocation[a]->getIndexes()[i].find(".php")  != std::string::npos)
                                {
                                    sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]), sd, 0);
                                    client->isSend = true;
                                    break;
                                }
                                else
                                { // another extension
                                    sendResponseTest(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, 1);
                                    client->isSend = true;
                                    break;
                                }
                            }
                            else
                                count++;
                        }
                        if (count == serverLocation[a]->getIndexes().size()) // not found this indexes
                            break;
                    }
                }
            }
            else if (file.good()) // if file
            {

                std::string path = serverLocation[a]->getRootPath() + client->ClientRequest->GetRequestURI();
                    std::cout << path;
                if (path.find(".php") != std::string::npos)
                {
                    std::cout << ">>> IM2 HERE  <<<\n";
                    sendResponseTest(cgi(path), sd, 0);
                    client->isSend = true;
                    break;
                }
                else if (serverLocation[a]->getAutoIndex()) // autoindex ON
                {
                    sendResponseTest(LandingPage(serverLocation[0]->getRootPath() + client->ClientRequest->GetRequestURI()), sd, 0);
                    client->isSend = true;
                    break;
                }
                else // check indexes
                {
                    size_t count = 0;
                    for (size_t i = 0; i < serverLocation[a]->getIndexes().size(); i++)
                    {
                        std::ifstream file(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]);
                        std::cout << "search index = " << serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i] << "\n";
                        std::cout << "file found = " << file.good() << "\n";
                        if (file.good()) // if found this index
                        {
                            std::cout << "im here\n";
                            if (serverLocation[a]->getIndexes()[i].find(".php")  != std::string::npos)
                            {
                                sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]), sd, 0);
                                client->isSend = true;
                                break;
                            }
                            else
                            { // another extension
                                sendResponseTest(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, 1);
                                client->isSend = true;
                                break;
                            }
                        }
                        else
                            count++;
                    }
                    if (count == serverLocation[a]->getIndexes().size()) // not found this indexes
                        break;
                }
            }
        }
        if (!client->isSend)
        {
            sendResponseTest("<h1 style=\"color:red\">NOT FOUND 404</h1>", sd, 0);
        }
    }
    exit(0);
    // if (client->ClientRequest->GetMethod() == "GET") // <<<<<<<<<========= CHECK METHOD "GET"
    // {
    //     client->isSend = true;
    //     for (std::vector<std::string>::iterator it = path.begin(); it != path.end(); it++)
    //     {
    //         std::ifstream file("/Users/asekkak/Desktop/webserv/wordpress" + client->ClientRequest->GetRequestURI()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== OPEN FILE AND CHECK EXTENSION
    //         if (client->ClientRequest->GetRequestURI() == *it || file.good()) // <<<<<<<<<<<<<<============ COMPARE RESOURCE WITH PATH LOCATION
    //         {                                                  // <<<<<<<<<<<<<<<<<<==================================== CHECK THE RESOUCR IS FILE OR DIRECTORY
    //             if (file.good() && client->ClientRequest->GetRequestURI().size() > 2)
    //             {
    //                 std::cout << "\n open file and execute to cgi";
    //                 if (client->ClientRequest->GetRequestURI().find(".php") != std::string::npos) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<===========  CHECK EXTENSION  [.PHP]
    //                 {
    //                     client->isSend = false;
    //                     sendResponseTest(cgi(client->ClientRequest->GetRequestURI()), sd, 0);
    //                     break;
    //                 }
    //                 else if(client->ClientRequest->GetRequestURI().find(".html") != std::string::npos)
    //                 {
    //                     sendResponseTest(client->ClientRequest->GetRequestURI(), sd, 1);
    //                     break;

    //                 }
    //             }
    //             else // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== SEARCH INDEX
    //             {
    //                 client->isSend = false;
    //                 std::cout << "size of indexes is : " << indexes.size() << " search in index\n";
    //                 for (size_t i = 0; i < indexes.size(); i++)
    //                 {
    //                     std::ifstream file("/Users/asekkak/Desktop/webserv/wordpress/" + indexes[i]);
    //                     if (file.good())
    //                     {
    //                         std::string file = "/" + indexes[i];
    //                         if (indexes[i].find(".html") != std::string::npos)
    //                         {
    //                             client->isSend = false;
    //                             sendResponseTest(indexes[i], sd, 1);
    //                     break;

    //                         }
    //                         else{
    //                             client->isSend = false;
    //                             sendResponseTest(cgi("/" + indexes[i]), sd, 0);
    //                     break;

    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     if(client->isSend)  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== NOT FOUND 404
    //     {
    //         sendResponseTest("<h1 style=\"color:red\">Not found 404</h1>", sd, 0);
    //     }

    // }
    // exit(0);
}