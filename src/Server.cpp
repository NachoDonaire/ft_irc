#include <Server.hpp>

Server::Server()
{
}

int	Server::reallocPollFd(int index)
{
	struct pollfd *neo;
	int				i;
	int				y;
	int				nClients;
	int				limit;

	if (index < 0)
	{
		neo = (struct pollfd *)malloc(sizeof(pollfd) * (nClientsOnline + 2));
		limit = nClientsOnline + 1;
		nClients = nClientsOnline + 1;
	}
	else
	{
		neo = (struct pollfd *)malloc(sizeof(pollfd) * (nClientsOnline));
		limit = nClientsOnline + 1;
		nClients = nClientsOnline - 1;
	}
	i = 0;
	y = 0;
	while (i < limit)
	{
		if (i == index)
		{
			i++;
			continue ;
		}
		neo[y].fd = clientSock[i].fd;
		neo[y].events = clientSock[i].events;
		neo[y++].revents = clientSock[i++].revents;
	}
	return (nClients);
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
		if (clientSock[y].revents & POLLIN)
		{
			int nread = recv(clientSock[y].fd, this->recData, 512, 0);
			this->recData[nread] = '\0';
			std::cout << "I am: " << y << std::endl;
			std::cout << recData << std::endl;
			std::cout << clients.size() << std::endl;
			clients[0].setMsg(recData);
		}
	}
	int	i = 0;
	while (++i < nClientsOnline + 1)
	{
		if (clientSock[i].revents & POLLHUP)
		{
			std::cout << "JUGGLES" << std::endl;
			nClientsOnline = reallocPollFd(i);
			close(clientSock[i].fd);
			std::cout << "JUGGLES" << std::endl;
			std::cout << nClientsOnline << " and i am: " << i <<std::endl;
			i--;
		}
	}

}

void	Server::handleError(int status, Client c)
{
	if (status == 1)
	{
		std::cerr << "Not valid command from " << c.getNick() << std::endl;
	}
	else if (status == 2)
	{
		//std::cerr << "Not valid psswd" << std::endl;
		std::string msgerr("Not valid password");
		send(c.getSocket(), msgerr.c_str(), msgerr.size(), 0);
		close(c.getSocket());
	}
}

void	Server::pass(Client c)
{
	std::string msg("PASS ");
	msg += psswd;
	msg += std::string("\r\n");
	send(c.getSocket(), msg.c_str(), msg.size(), 0);
}

void	Server::launchAction(Client c)
{
	//if (c.getParams()[0] == "PASS")
	{
		pass(c);
	}
}

void	Server::handleMessages()
{
	std::vector<Client>::iterator y;
	int							parseStatus;

	for (y = clients.begin(); y != clients.end(); y++)
	{
		if (y->getMsg() != "")
		{
			parseStatus = y->parseMsg();
			if (parseStatus != 0)
			{
				handleError(parseStatus, *y);
				return ;
			}
			this->launchAction(*y);
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


