#include <ft_common.h>
#include "Config.hpp"
#include "Request.hpp"

void post_response(int sd, Client *client, const std::vector<Location *> &serverLocation, std::map<int, Client *> ClientsInformation)
{
    (void)sd;
    (void)serverLocation;
    (void)client;
    (void)ClientsInformation;
    if (client->_clientRequest->_Files.size() > 0)
    {
        // std::cout << "post2";
        for (size_t i = 0; i < serverLocation.size(); i++)
        {
            if (!serverLocation[i]->getUploadDir().empty())
            {
                // std::ofstream file(serverLocation[i]->getUploadDir() + "/" + client->_clientRequest->_Files[0].first);
                // file << client->_clientRequest->_Files[0].second;
                // std::ostringstream http_response;
                // http_response << "HTTP/1.1 201 Created!\r\nContent-Type: text/html\r\n\n <h1>upload is completed</h1>";
                // http_response << client->_clientRequest->_Files[0].second;
                CreateFiles(client->_clientRequest);
                sendResponseTest("Created!", sd, 0);
                break;
            }
        }
    }
    else
    {
        // std::cout << "post3";
        get_response(sd, client, serverLocation, ClientsInformation);
    }
}
