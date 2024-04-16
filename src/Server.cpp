#include <Server.hpp>

Server::Server()
{
}

int	Server::reallocPollFd(int index)
{
	if (index < 0)
	{
		struct pollfd	*neopfd;
		int		i;

		i = 0;
		// +2 --> uno por el cliente nuevo y otro por el listener
		neopfd = (struct pollfd *)malloc(sizeof(pollfd) * (nClientsOnline + 2));
		while (i < nClientsOnline + 1) 
		{
			neopfd[i].fd = clientSock[i].fd;
			neopfd[i].events = clientSock[i].events;
			neopfd[i].revents = clientSock[i].revents;
			i++;
		}
		free(clientSock);
		clientSock = neopfd;
		return (nClientsOnline + 1);
	}
	
	struct pollfd	*neopfd;
	int		i;
	int		y;

	i = 0;
	y = 0;
	// +2 --> uno por el cliente nuevo y otro por el listener
	neopfd = (struct pollfd *)malloc(sizeof(pollfd) * (nClientsOnline));
	while (i < nClientsOnline + 1) 
	{
		if (i == index)
		{
			i++;
			continue ;
		}
		neopfd[y].fd = clientSock[i].fd;
		neopfd[y].events = clientSock[i].events;
		neopfd[y].revents = clientSock[i].revents;
		i++;
		y++;
	}
	free(clientSock);
	clientSock = neopfd;
	return (nClientsOnline - 1);
}

Server::Server(char *p, char *pd) : port(p)
{
	psswd = std::string(pd);
	nClientsOnline = 0;
	// 1 para el listener
	this->clientSock = (struct pollfd *)malloc(sizeof(pollfd) * (1));
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
	std::cout << nClientsOnline << std::endl;
	clientSock[nClientsOnline].fd = accept(serverSock, rp->ai_addr, &rp->ai_addrlen);
	clientSock[nClientsOnline].events = POLLIN;
	clients.push_back(Client(clientSock[nClientsOnline].fd, psswd));
}

void	Server::checkClientEvents()
{
	for (int y = 1; y < nClientsOnline + 1; y++)
	{
		if (clientSock[y].revents & POLLIN && clientSock[y].fd != -1)
		{
			int nread = recv(clientSock[y].fd, this->recData, 512, 0);
			this->recData[nread] = '\0';
			std::cout << "I am: " << y << std::endl;
			std::cout << recData << std::endl;
			clients[y - 1].setMsg(std::string(recData));
		}
	}
}

void	Server::handleMessages()
{
	std::vector<Client>::iterator y;

	for (y = clients.begin(); y != clients.end(); y++)
	{
		if (y->getMsg() != "")
		{
			y->parseMsg();
			y->setMsg("");
		}
	}
}
			


bool	Server::handleConnections()
{
	bool		error = 0;
	while (error == 0)
	{
		//harcodeo de nclientes + server sockets y el timeout
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


