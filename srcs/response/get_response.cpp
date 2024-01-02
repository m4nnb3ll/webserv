#include <ft_common.h>
#include "Config.hpp"
#include "Request.hpp"
#include "Client.hpp"

void handle_response(int sd, Client *client, std::string clientPath, Location *serverLocation, std::map<int, Client *> ClientsInformation)
{
    (void)clientPath;
    // check is file or directory
    std::cout << "Path is redirect to directory\n";
    std::string pathDir = serverLocation->getRootPath() + client->_clientRequest->_getRequestURI();
    DIR *dir = opendir(pathDir.c_str());       // check is directory or file
    if (dir && serverLocation->getAutoIndex()) // if autoindex is on
    {
        sendResponseTest(LandingPage(serverLocation->getRootPath() + client->_clientRequest->_getRequestURI(), client->_clientRequest->_getRequestURI()), sd, 0);
        client->_isSend = true;
    }
    else if (!serverLocation->getAutoIndex() && dir) // search of indexes
    {
        for (size_t i = 0; i < serverLocation->getIndexes().size(); i++)
        {
            std::cout << "search index = " << serverLocation->getRootPath() + "/" + serverLocation->getIndexes()[i] << "\n";
            std::ifstream file(serverLocation->getRootPath() + "/" + serverLocation->getIndexes()[i]);
            if (file.good())
            {
                std::map<std::string, std::string> isCgi = serverLocation->getCgi();
                std::map<std::string, std::string>::iterator it = isCgi.begin();
                if (serverLocation->getIndexes()[i].find(".php") != std::string::npos && !isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php") // check cgi if exist
                {
                    std::cout << serverLocation->getRootPath() + "/" + client->_clientRequest->_getRequestURI();
                    sendResponseTest(cgi(serverLocation->getRootPath() + "/" + serverLocation->getIndexes()[i], sd, serverLocation, ClientsInformation), sd, 0);
                    client->_isSend = true;
                }
                else
                {
                    sendResponseTest(serverLocation->getRootPath() + "/" + serverLocation->getIndexes()[i] , sd, 1);
                    client->_isSend = true;
                }
                break;
            }
        }
        std::cout << client->_isSend;
        if(!client->_isSend)
            sendResponseTest("<h1 style=\"color:red\">NOT FOUND 404</h1>", sd, 0);
    }
    std::ifstream file(pathDir);
    if (file.good() && client->_isSend == 0)
    {
        std::cout << "IM herererer";
        std::map<std::string, std::string> isCgi = serverLocation->getCgi();
        std::map<std::string, std::string>::iterator it = isCgi.begin();
        if (client->_clientRequest->_getRequestURI().find(".php") != std::string::npos && !isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php") // check cgi if exist
        {
            std::cout << serverLocation->getRootPath()   + client->_clientRequest->_getRequestURI();
            sendResponseTest(cgi(serverLocation->getRootPath() + "/" + client->_clientRequest->_getRequestURI(), sd, serverLocation, ClientsInformation), sd, 0);
        }
        else
            sendResponseTest("not php and prepare for this request (may to be index.html must to show file))", sd, 0);
        client->_isSend = true;
    }
}

void get_response(int sd, Client *client, const std::vector<Location *> &serverLocation, std::map<int, Client *> ClientsInformation)
{
    (void)ClientsInformation;
    (void)sd;
    int count_slach = 0;
    int size = 0;

    // std::cout << client->_clientRequest->_getRequestURI()<< "\n";
    for (int i = 0; client->_clientRequest->_getRequestURI()[i]; i++) // use used to count how many slach in path and size this path
    {
        // std::cout << client->_clientRequest->_getRequestURI()[i] << "\n";
        if (client->_clientRequest->_getRequestURI()[i] == '/')
            count_slach += 1;
        size++;
    }
    std::string clientPath = client->_clientRequest->_getRequestURI();
    for (int i = 0; i < count_slach && !client->_isFound; i++) // chunks path to compare with location path
    {
        // std::cout << "-> " << clientPath << "\n"; // check path
        for (size_t a = 0; a < serverLocation.size() && !client->_isFound; a++)
        {
            std::cout << clientPath << "  " << serverLocation[a]->getPath() << "\n";
            if (clientPath == serverLocation[a]->getPath()) // ===> if equal
            {
                std::cout << " ======================== >>>>>. there is equal path \n";
                handle_response(sd, client, clientPath, serverLocation[a], ClientsInformation);
                client->_isFound = true;
                break;
            }
        }

        for (int j = 0; size > 0 && !client->_isFound; size--)
        {
            (void)j;

            if (clientPath[size] == '/')
            {
                clientPath = clientPath.substr(0, size);
                break;
            }
        }
    }

    for (size_t a = 0; a < serverLocation.size() && !client->_isFound; a++) // when not found any similar path , must to check if found slach /
    {
        if (serverLocation[a]->getPath() == "/") // ===> if equal
        {
            std::cout << " ======================== >>>>>. there is slach in location path \n";
                handle_response(sd, client, clientPath, serverLocation[a], ClientsInformation);
            client->_isFound = true;
        }
    }

    if (!client->_isFound)
    {
        sendResponseTest("<h1 style=\"color:red\">NOT FOUND 404</h1>", sd, 0);
    }

    // std::cout << "count_slach " << count_slach << "\n";
 
}






















 // exit(0);
    // in beginner check the path in location or not

    // for (size_t a = 0; a < serverLocation.size() && !client->_isSend && client; a++)
    // {
    //     if (client->_clientRequest->_getRequestURI() == serverLocation[a]->getPath())    // if / == /
    //     {
    //         std::cout << "Path is redirect to directory\n";
    //         std::string pathDir = serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI();
    //         DIR *dir = opendir(pathDir.c_str()); // check is directory or file
    //         if(dir && serverLocation[a]->getAutoIndex()) // if autoindex is on
    //         {
    //             sendResponseTest(LandingPage(serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI()), sd, 0);
    //             client->_isSend = true;
    //             break;
    //         }
    //         else if(!serverLocation[a]->getAutoIndex()) // search of indexes
    //         {
    //             for(size_t i = 0; i < serverLocation[a]->getIndexes().size(); i++)
    //             {
    //                 std::cout << "search index = " << serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i] << "\n";
    //                 std::ifstream file(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]);
    //                 if(file.good())
    //                 {
    //                     std::map<std::string, std::string> isCgi = serverLocation[a]->getCgi();
    //                     std::map<std::string, std::string>::iterator it = isCgi.begin();
    //                     if(serverLocation[a]->getIndexes()[i].find(".php") != std::string::npos && !isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php") // check cgi if exist
    //                     {
    //                         sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, serverLocation[a], ClientsInformation), sd, 0);
    //                     }
    //                     else
    //                         sendResponseTest("not php and prepare for this request (may to be index.html must to show file))", sd, 0);
    //                     client->_isSend = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }
    // std::ifstream file(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]);

    // if(file.good())
    //                 {
    //                     std::map<std::string, std::string> isCgi = serverLocation[a]->getCgi();
    //                     std::map<std::string, std::string>::iterator it = isCgi.begin();
    //                     if(serverLocation[a]->getIndexes()[i].find(".php") != std::string::npos && !isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php") // check cgi if exist
    //                     {
    //                         sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, serverLocation[a], ClientsInformation), sd, 0);
    //                     }
    //                     else
    //                         sendResponseTest("not php and prepare for this request (may to be index.html must to show file))", sd, 0);
    //                     client->_isSend = true;
    //                     break;
    //                 }
    // if(!client->_isSend && client)
    //     sendResponseTest("<h1 style=\"color:red\">NOT FOUND 404</h1>", sd, 0);
    // std::ifstream file((serverLocation[0]->getRootPath() + client->_clientRequest->_getRequestURI()).c_str()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== OPEN FILE AND CHECK EXTENSION

    // for (size_t a = 0; a < serverLocation.size(); a++)
    // {
    //     std::map<std::string, std::string> isCgi = serverLocation[a]->getCgi();
    //     std::map<std::string, std::string>::iterator it = isCgi.begin();
    //     std::ifstream file((serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI()).c_str()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=========== OPEN FILE AND CHECK EXTENSION
    //     if (client->_clientRequest->_getRequestURI() == serverLocation[a]->getPath() && client->_isSend != true)    // if / == /
    //     {                                                                                                        // handle if autoindex on or off , and check is file or not
    //         std::cout << "dir";
    //         std::vector<std::string> path;
    //         std::string pathDir = serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI();
    //         DIR *dir = opendir(pathDir.c_str());

    //         path.push_back(client->_clientRequest->_getRequestURI());
    //         if ((dir || path.back() == "/") && client->_isSend != true) // if a directory
    //         {
    //             std::cout << "is a directory\n";
    //             if (serverLocation[a]->getAutoIndex()) // autoindex ON
    //             {

    //                 sendResponseTest(LandingPage(serverLocation[a]->getRootPath() + client->_clientRequest->_getRequestURI()), sd, 0);
    //                 client->_isSend = true;
    //                 break;
    //             }
    //             else if (client->_isSend != true) // check indexes
    //             {
    //                 size_t count = 0;
    //                 for (size_t i = 0; i < serverLocation[a]->getIndexes().size(); i++)
    //                 {
    //                     std::cout << "------------";
    //                     std::ifstream file((serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i]).c_str());
    //                     std::cout << "search index = " << serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i] << "\n";
    //                     // std::cout << "file found = " << file.good() << "\n";
    //                     if (file.good()) // if found this index
    //                     {
    //                         std::cout << "im here\n";
    //                         if (serverLocation[a]->getIndexes()[i].find(".php") != std::string::npos)
    //                         {
    //                             if (!isCgi.empty() && it->first == ".php" && it->second == "/usr/bin/php")
    //                             {
    //                                 sendResponseTest(cgi(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, serverLocation[a], ClientsInformation), sd, 0);
    //                                 client->_isSend = true;
    //                                 break;
    //                             }
    //                             else
    //                                 sendResponseTest("internal server error 500", sd, 0);
    //                             break;
    //                         }
    //                         else
    //                         { // another extension
    //                             sendResponseTest(serverLocation[a]->getRootPath() + "/" + serverLocation[a]->getIndexes()[i], sd, 1);
    //                             client->_isSend = true;
    //                             break;
    //                         }
    //                     }
    //                     else
    //                         count++;
    //                 }
    //             }
    //         }
    //     }
    // }
    // if (client && !client->_isSend)
    // {
    //     sendResponseTest("<h1 style=\"color:red\">NOT FOUND 404</h1>", sd, 0);
    // }