#include <Server.hpp>
#include <stdio.h>

Server::Server()
{
}

std::string	Server::msgGenerator(int msg, std::vector<std::string> params, Client c)
{
	std::string response(hostName);

	std::cout << c.getStatus() << std::endl;
	if (msg == CMD_NOTFOUND)
	{
		response += " 421 " + params[0] + " :Unknown command\r\n";
	}
	else if (msg == NICK_OK)
	{
		response += " 381 " + c.getNick() + " : Capabilities displayed properly !\r\n";
	}
	else if (msg == NICK_REPEATED)
	{
		response += " NICK :" + params[1] + "\r\n";
	}
	else if (msg == QUIT)
	{
		if (params.size() <= 1)
			response += " QUIT: No leaving message";
		else
		{
			std::string quitMsg(&params[1].c_str()[1]);
			response += " QUIT: ";
			response += quitMsg;
		}
	}
	else if (msg == WELCOME && c.getStatus() == REGISTER_PENDING)
	{
		response += " 001 " + c.getNick() + " :Welcome to ircserv !\r\n";
	}
	if (msg == ERROR)
	{
		response += " 461 ";
		response += params[0];
		response += " :Not enough or too much params for ";
		response += params[0];
		response += "\r\n";
	}

	return (response);
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

Server::Server(char *p, char *pd, std::string hn) : port(p)
{
	psswd = std::string(pd);
	nClientsOnline = 0;
	hostName = hn;
	
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
	this->clientSock[0].events = (POLLIN);// | POLLOUT);
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
	clientSock[nClientsOnline].events = (POLLIN); 
	clients.push_back(Client(clientSock[nClientsOnline].fd, nClientsOnline, psswd, hostName));
}

void	Server::checkClientEvents()
{
	std::vector<int>	pos;
	//std::cout << "alakazo" << std::endl;
	
	for (int y = 1; y < nClientsOnline + 1; y++)
	{
		std::cout << "revents: " << clientSock[y].revents << std::endl;
		if (clientSock[y].revents & POLLIN)
		{
			int nread = recv(clientSock[y].fd, this->recData, 512, 0);
			if (nread <= 0)
			{
				std::cout << "aqui1" << std::endl;
				pos.push_back(y);
				continue ;
			}
			this->recData[nread] = '\0';
			std::cout << recData << std::endl;
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
			//std::cout << "vamosaver" << std::endl;
			//std::cout << clients[y - 1].getCmd().size() << std::endl;
			//std::map<int, std::vector<std::string> >::iterator it = clients[y - 1].getCmd().end();
			//clients[y - 1].printCmd();
			//for (std::vector<std::string>::iterator it = clients[y - 1].getResponses().begin(); it != clients[y - 1].getResponses().end(); it++)
			for (size_t i = 0; i < clients[y - 1].getResponses().size(); i++)
			{

			//std::cout << "response :" << std::endl;
			//	std::cout << response << std::endl;
				send(clients[y - 1].getSocket(), aux[i].c_str(), aux[i].size(), 0);
				std::cout << "charResponse: ";
				printf("%s\n", aux[i].c_str());
			}
			clients[y - 1].emptyResponse();
			//(void)it;
				std::cout << "kaka2" << std::endl;
			clients[y - 1].setPollOut(0);
		}
	}
	std::vector<Client>::iterator y = clients.begin();
	for (std::vector<int>::iterator it = pos.begin(); it != pos.end(); it++)
	{
				//std::cout << "aqui2" << std::endl;
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

void	Server::mark(Client *c, std::string msg, std::map<int, std::vector<std::string> > cm)
{
	Command cd(c, clients, hostName, msg, psswd, cm);
	std::cout << "pollout: " << c->getPollOut() << std::endl;
	
	cd.handleCmd();
	//return (cd);
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
				continue ;
			}
			this->mark(&(*(y)),  y->getMsg(), y->getCmd());
			y->setMsg("");
		}
	}
}
			


bool	Server::handleConnections()
{
	while (1)
	{
		for (int y = 1; y < nClientsOnline + 1; y++)
		{
			if (clients[y - 1].getPollOut() == 1)
			{
				clientSock[y].events = (POLLIN | POLLOUT);
			}
		}
		if (poll(clientSock, nClientsOnline + 1, 3000) < 0)
		{
			std::cerr << "poll() error" << std::endl;
			return 1;
		}
		if (clientSock[0].revents & POLLIN)
		{
			establishConnection();
		}
		checkClientEvents(); // QUTAMOS POLLOUT EVENTS
		handleMessages(); // MARCAMOS FLAG POLLOUT
		for (int y = 1; y < nClientsOnline + 1; y++)
		{
			if (clients[y - 1].getPollOut() == 0)
				clientSock[y].events = (POLLIN);
		}
	

	
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


