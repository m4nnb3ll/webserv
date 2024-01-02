#include "Config.hpp"
#include "Request.hpp"

void Config::_readRequest(int sd)
{

	// The server-name WILL be checked after reading the request

    char	buffer[BUFFER_SIZE];
	int		ret;

    memset(buffer, 0, BUFFER_SIZE);
	ret = recv(sd, buffer, BUFFER_SIZE - 1, 0);

	if (!ret || ret == -1) {
		_rmPollfd(sd);
		std::cout << "reading: client[" << sd <<"] disconnected" << std::endl;
		return;
	}
	_readStr += buffer;
}

// void Config::_sendResponse(int sd, std::map<int, Client *>  ClientsInformation)
// {
// 	(void)ClientsInformation;
// 	std::ostringstream	http_response;

// 	http_response << "HTTP/1.1 200 OK\r\n";
// 	http_response << "Content-Type: image/jpeg\r\n";
// 	std::ifstream file("assets/test_image.jpg"); // Replace with your file path
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     std::string content = buffer.str();
// 	// std::string	content(fileContent);
// 	http_response << "Content-Length: " << content.size();
// 	http_response << "\r\n\r\n" << content;

// 	int ret = send(sd, http_response.str().c_str(), http_response.str().size(), 0);
// 	if (ret == -1) {
// 		_rmPollfd(sd);
// 		std::cout << "writing: client[" << sd <<"] disconnected" << std::endl;
// 		return;
// 	}
// }

void Config::run()
{
	Printers::print_serversSockets(_portToServersSocket);

	ServersSocket* 				sS;
	std::map<int, Client *>		ClientsInformation;

	while (!g_sigint)
	{
		if ((poll(_pollFds.data(), _pollFds.size(), -1)) <= 0)
			continue;
		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			int sd = _pollFds[i].fd;
			if ((_pollFds[i].revents & POLLERR)
				|| _pollFds[i].revents & POLLHUP)
			{
				if ((sS = _sdToServersSocket[sd]) && sS->getFd() == sd)
					_unplugSocket(sS);
				_rmPollfd(sd);
				std::cerr << "POLL QUIT " << sd << std::endl;
			}
			else if (_pollFds[i].revents & POLLIN)
			{
				if ((sS = _sdToServersSocket[sd]) && sS->getFd() == sd)
				{
					sockaddr_in client_addr;
					socklen_t client_addr_len = sizeof(client_addr);
					int client_fd = accept(sd, (struct sockaddr *)&client_addr, &client_addr_len);
					if (client_fd < 0) continue;
					_sdToServersSocket[client_fd] = sS;
					_addPollfd(client_fd, POLLIN | POLLOUT);
				}
				else // read request from client
				{
					_readRequest(sd);
					if (HandleRequest(_readStr, sd, &ClientsInformation))
						_readStr = "";
				}
			}
			else if (_pollFds[i].revents & POLLOUT) // send response to the client
			{
				Client *cli = ClientsInformation[sd];
				if(cli && cli->_clientRequest->_isFinished)
				{
 					_sendResponse(sd, ClientsInformation);
				}
			 
 			}
		}
	}
}