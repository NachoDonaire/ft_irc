#include <Server.hpp>

Server::Server()
{
}

void	Server::printpfd(struct pollfd *src, int size)
{
	int	i = 0;
	std::cout << "FDS" << std::endl;
	while (i < size)
	{
		std::cout << src[i++].fd << std::endl;
	}
}



void	Server::pollfdcpy(struct pollfd *src, int size)
{
	int	i = 0;
	this->clientSock = (struct pollfd *)malloc(sizeof(pollfd) * (size));
	while (i < size)
	{
		clientSock[i].fd = src[i].fd;
		clientSock[i].events = src[i].events;
		clientSock[i].revents = src[i].revents;
		i++;
	}
}

int	Server::reallocPollFd(int index)
{
	int	size = index < 0 ? nClientsOnline + 2 : nClientsOnline;
	int	nClients = index < 0 ? nClientsOnline + 1 : nClientsOnline - 1;
	struct pollfd neopfd[size];
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (i < nClientsOnline + 1)
	{
		if (i == index)
		{
			i++;
			continue ;
		}
		neopfd[y].fd = clientSock[i].fd;
		neopfd[y].events = clientSock[i].events;
		neopfd[y++].revents = clientSock[i++].revents;
	}
	free(clientSock);
	this->clientSock = (struct pollfd *)malloc(sizeof(struct pollfd) * (size));
	memcpy(clientSock, neopfd, sizeof(struct pollfd) * size);
	//pollfdcpy(neopfd, size);
	return nClients;
}

Server::Server(char *p, char *pd) : port(p)
{
	psswd = std::string(pd);
	nClientsOnline = 0;
	// 1 para el listener
	this->clientSock = (struct pollfd *)malloc(sizeof(pollfd) * (3));
	//commands["PASS"] = &Server::pass;
}	

bool	Server::launchServer()
{
	struct addrinfo *result, hints;

	hints.ai_family = AF_INET; //ipv4
	hints.ai_socktype = SOCK_STREAM; /* tcp */
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;          /* Any protocol */
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

	if (getaddrinfo(NULL, this->port, &hints, &result) != 0)
	{
		std::cerr << "getaddrinfo error" << std::endl;
		return 1;
	}
	for (this->rp = result; this->rp != NULL; this->rp = this->rp->ai_next)
	{
		this->serverSock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (this->serverSock == -1)
			continue ;
		if (bind(serverSock, rp->ai_addr, rp->ai_addrlen) == 0)
			break ;
	}
	freeaddrinfo(result);
	if (rp == NULL)
	{
		std::cerr << "Cannot bind" << std::endl;
		return 1;
	}
	//otro hardcodeo de un 5 que refiere al maximo numeros de posibles conexiones | hay que verlo
	this->clientSock[0].fd = serverSock;
	this->clientSock[0].events = POLLIN;
	listen(serverSock, 5);
	return (this->handleConnections());
}

void	Server::establishConnection()
{
	std::cout << "Someones connecting" << std::endl;
	nClientsOnline = reallocPollFd(-1);
	//nClientsOnline++;
	std::cout << nClientsOnline << std::endl;
	clientSock[nClientsOnline].fd = accept(serverSock, rp->ai_addr, &rp->ai_addrlen);
	clientSock[nClientsOnline].events = POLLIN;
	clients.push_back(Client(clientSock[nClientsOnline].fd, nClientsOnline, psswd));
}

void	Server::checkClientEvents()
{
	std::vector<int>	pos;
	//std::cout << "alakazo" << std::endl;
	for (int y = 1; y < nClientsOnline + 1; y++)
	{
		if (clientSock[y].revents & POLLIN)
		{
			int nread = recv(clientSock[y].fd, this->recData, 512, 0);
			if (nread <= 0)
			{
				pos.push_back(y);
				continue ;
			}
			this->recData[nread] = '\0';
			//std::cout << "I am: " << y << std::endl;
			//std::cout << recData << std::endl;
			//std::cout << clients.size() << std::endl;
			clients[y - 1].setMsg(recData);
		}
		if (clientSock[y].revents & (POLLHUP | POLLERR | POLLNVAL))
		{
				pos.push_back(y);
				continue ;
		}
	}
	std::vector<Client>::iterator y = clients.begin();
	for (std::vector<int>::iterator it = pos.begin(); it != pos.end(); it++)
	{
		close(clientSock[*it].fd);
		clients.erase(y + (*it - 1));
		nClientsOnline = reallocPollFd(*it);
	}

}

void	Server::handleError(int status, Client c)
{
	if (status == 1)
	{
		std::cerr << "Not valid command from " << c.getId() << std::endl;
	}
	else if (status == 2)
	{
		//std::cerr << "Not valid psswd" << std::endl;
		std::string msgerr("Not valid password");
		send(c.getSocket(), msgerr.c_str(), msgerr.size(), 0);
		close(c.getSocket());
	}
}

void	Server::pass(Client c) const
{
	std::string msg("PASS ");
	msg += psswd;
	msg += std::string("\r\n");
	send(c.getSocket(), msg.c_str(), msg.size(), 0);
}

void	Server::launchAction(Client c)
{
	c.handleCmd();
}

void	Server::handleMessages()
{
	std::vector<Client>::iterator y;
	int							parseStatus;

	for (y = clients.begin(); y != clients.end(); y++)
	{
		if (y->getMsg() != "")// && y->getParseStatus() == 1)
		{
			parseStatus = y->parseMsg();
			if (parseStatus != 0)
			{
				//handleError(parseStatus, *y);
				//return ;
				continue ;
			}
			//std::cout << "ee" << std::endl;
			this->launchAction(*(y));
			//y->setMsg("");
		}
	}
}
			


bool	Server::handleConnections()
{
	while (1)
	{
		if (poll(clientSock, nClientsOnline + 1, 50000) < 0)
		{
			std::cerr << "poll() error" << std::endl;
			return 1;
		}
		if (clientSock[0].revents & POLLIN)
		{
			establishConnection();
		}
		checkClientEvents();
		handleMessages();
	
	}
	return 0;
}

int Server::lookForAvailableSocket()
{
	for (int i = 1; i < 6; i++)
	{
		if (this->clientSock[i].fd == -1)
			return i;
	}
	return -1;
}





Server::~Server()
{
}


