#include <Server.hpp>
#include <stdio.h>

#define POLLFD_LIMIT -7

Server::Server()
{
}

int	Server::pollfdLen()
{
	int	i = 0;

	while (this->clientSock[i].fd != POLLFD_LIMIT)
		i++;
	return (i);
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


int	Server::reallocPollFd(int index)
{
	int	size = index < 0 ? currentSize + 1 : currentSize - 1;
	struct pollfd neopfd[size];
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (i < currentSize)
	{
		if (i == index)
		{
			i++;
			continue;
		}
		neopfd[y].fd = clientSock[i].fd;
		neopfd[y].events = clientSock[i].events;
		neopfd[y++].revents = clientSock[i++].revents;
	}
	free(clientSock);
	this->clientSock = (struct pollfd *)malloc(sizeof(struct pollfd) * (size + 1));
	memcpy(clientSock, neopfd, sizeof(struct pollfd) * (size + 1));
	this->clientSock[size].fd = POLLFD_LIMIT;
	currentSize = pollfdLen();
	//pollfdcpy(neopfd, size);
	return size;
}

Server::Server(char *p, char *pd, std::string hn) : port(p)
{
	psswd = std::string(pd);
	hostName = hn;
	
	// 1 para el listener
	this->clientSock = (struct pollfd *)malloc(sizeof(pollfd) * (2));
	this->clientSock[1].fd = -7;
	currentSize = pollfdLen();
	//std::cout << "CURRENT SIZE AT CONSTRUCTOR" << currentSize <<std::endl;
	//clients.push_back(Client(clientSock[currentSize].fd, currentSize, psswd, hostName));
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
	this->clientSock[0].events = (POLLIN);// | POLLOUT);
	listen(serverSock, 5);
	return (this->handleConnections());
}

void	Server::establishConnection()
{
	std::cout << "Someones connecting" << std::endl;
	currentSize = reallocPollFd(-1);
	//currentSize++;
	std::cout << currentSize - 1 << std::endl;
	clientSock[currentSize - 1].fd = accept(serverSock, rp->ai_addr, &rp->ai_addrlen);
	if (clientSock[currentSize - 1].fd < 0)
	{
		currentSize = reallocPollFd(currentSize);
	}
	else
	{
		clientSock[currentSize - 1].events = (POLLIN); 
		clients.push_back(Client(clientSock[currentSize - 1].fd, currentSize, psswd, hostName));
	}
	//std::cout << "Brand new Cadillac" << std::endl;
}

void	Server::checkClientEvents()
{
	std::vector<int>	pos;
	//std::cout << "alakazo" << std::endl;
	
	for (int y = 1; y < currentSize; y++)
	{
		//std::cout << "revents " << clients[y - 1].getSocket() << " : " << clientSock[y].revents << std::endl;
		if (clientSock[y].revents & POLLIN)
		{
			int nread = recv(clientSock[y].fd, this->recData, 512, 0);
			if (nread <= 0)
			{
				//std::cout << "aqui1" << std::endl;
				pos.push_back(y);
				continue ;
			}
			this->recData[nread] = '\0';
			//std::cout << recData << std::endl;
			clients[y - 1].setMsg(recData);
			if (clients[y - 1].getRegister() == 2)
				clients[y - 1].setPollOut(1);
		}
		if (clientSock[y].revents & (POLLHUP | POLLERR | POLLNVAL))
		{
				pos.push_back(y);
				continue ;
		}
		if (clientSock[y].revents & POLLOUT)
		{
			std::string response;
			std::vector<std::string> aux = clients[y - 1].getResponses();
			std::vector<int> NCmd = clients[y - 1].getNCmd();
			//std::cout << "size of both : " << aux.size() << NCmd.size() << std::endl;
			for (size_t i = 0; i < clients[y - 1].getResponses().size(); i++)
			{
				send(clients[y - 1].getSocket(), aux[i].c_str(), aux[i].size(), 0);
				//std::cout << aux[i] << std::endl;
				if (NCmd[i] == BAD_PSSWD || NCmd[i] == QUIT)
				{
					//std::cout << "ave" << std::endl;
					pos.push_back(y);
				}
			}
			clients[y - 1].emptyResponse();
			clients[y - 1].emptyNCmd();
			clients[y - 1].setPollOut(0);
		}
	}
	std::vector<Client>::iterator y = clients.begin();
	for (std::vector<int>::iterator it = pos.begin(); it != pos.end(); it++)
	{
		close(clientSock[*it].fd);
		currentSize = reallocPollFd(*it);
		clients.erase(y + (*it - 1));
	}

}

void	Server::pass(Client c) const
{
	std::string msg("PASS ");
	msg += psswd;
	msg += std::string("\r\n");
	send(c.getSocket(), msg.c_str(), msg.size(), 0);
}

void	Server::mark(Client *c, std::string msg, std::map<int, std::vector<std::string> > cm)
{
	Command cd(c, this->getClients(), hostName, msg, psswd, cm);
	

	cd.handleCmd();
	//cd.printShait();
}

void	Server::handleMessages()
{
	std::vector<Client>::iterator y;
	int							parseStatus;

	for (y = clients.begin(); y != clients.end(); y++)
	{
		if ((*y).getMsg() != "")
		{
			parseStatus = (*y).parseMsg();
			if (parseStatus != 0)
			{
				continue ;
			}
			this->mark(&(*(y)),  (*y).getMsg(), (*y).getCmd());
			(*y).emptyCmd();
			(*y).setMsg("");
		}
	}
}
			


bool	Server::handleConnections()
{
	while (1)
	{
		for (int y = 1; y < currentSize; y++)
		{
			if (clients[y - 1].getPollOut() == 1)
			{
				clientSock[y].events = (POLLIN | POLLOUT);
			}
		}
		if (poll(clientSock, currentSize, 3000) < 0)
		{
			std::cerr << "poll() error" << std::endl;
			return 1;
		}
		//std::cout << "ave1" << std::endl;
		if (clientSock[0].revents & POLLIN)
		{
			establishConnection();
		}
		//std::cout << "ave2" << std::endl;
		checkClientEvents(); // QUTAMOS POLLOUT EVENTS
		//std::cout << "ave3" << std::endl;
		handleMessages(); // MARCAMOS FLAG POLLOUT
		//std::cout << "ave4" << std::endl;
		for (int y = 1; y < currentSize; y++)
		{
			if (clients[y - 1].getPollOut() == 0)
				clientSock[y].events = (POLLIN);
		}
	

	
	}
	return 0;
}

Server::~Server()
{
	std::cout << "alo" << std::endl;
}


