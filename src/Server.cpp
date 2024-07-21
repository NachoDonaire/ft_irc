#include <Server.hpp>
#include <cerrno>
#include <stdio.h>

#define POLLFD_LIMIT -7

Server::Server()
{
	channels = vectorCh(0);
}

void	Server::printClientSock()
{
	std::cout << "CLIENTSOCK" << std::endl;
	for (std::vector<struct pollfd>::iterator it = clientSock.begin(); it != clientSock.end(); it++)
	{
		std::cout <<" fd : " <<  it->fd << std::endl;
		std::cout <<" events : " <<  it->events << std::endl;
		std::cout <<" revents : " <<  it->revents << std::endl;
	}
	std::cout << "-----------" << std::endl;
}

Server::Server(char *p, char *pd, std::string hn) : port(p)
{
	psswd = std::string(pd);
	port = std::string(p);
	hostName = hn;
	portError = 0;
	for (std::string::iterator c = port.begin(); c != port.end(); c++)
	{
		if (!std::isdigit(*c))
		{
			portError = 1;
			return ;
		}
	}
	currentSize = 1;
	std::cout << "CURRENT SIZE AT CONSTRUCTOR" << currentSize <<std::endl;
}	

bool	Server::launchServer()
{
	struct addrinfo *result, hints;
	struct pollfd listener;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;     // Dirección IP local

	if (getaddrinfo(NULL, port.c_str(), &hints, &result) != 0)
	{
		std::cerr << "getaddrinfo error" << std::endl;
		return 1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		serverSock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (serverSock == -1)
			continue;
		if (bind(serverSock, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(serverSock);
	}

	freeaddrinfo(result);

	if (serverSock == -1)
	{
		std::cerr << "Cannot bind" << std::endl;
		return 1;
	}

	listen(serverSock, 50);

	listener.fd = serverSock;
	listener.events = POLLIN;
	clientSock.push_back(listener);
	return handleConnections();
}

void	Server::reallocPlus()
{
	struct 	pollfd neoclient;
	int	socket;
	struct sockaddr_in address;
	socklen_t csin_len = sizeof(address);

	socket = accept(clientSock.at(0).fd,(struct sockaddr *)&address, &csin_len);
	if (socket < 0)
	{
	//	std::cerr << "cannot accept" << std::endl;
		std::cerr << "cannot accept: " << strerror(errno) << std::endl;
		return ;
	}
	neoclient.fd = socket;
	neoclient.events = POLLIN;
	clientSock.push_back(neoclient);
	clients.push_back(Client(socket, currentSize, psswd, hostName));
	currentSize++;
}

void	Server::establishConnection()
{
	std::cout << "Someones connecting" << std::endl;
	reallocPlus();
	std::cout << "His position is : " << currentSize - 1 << std::endl;
}

void	Server::checkClientEvents()
{
	std::vector<Client>::iterator	sit = clients.begin();
	std::vector<int>	out;
	int			pos;

	//POSIBLE SEGFAULT
	if (clientSock.size() <= 1)
		return ;
	pos = 0;
	for (std::vector<struct pollfd>::iterator it = clientSock.begin() + 1; it != clientSock.end(); it++)
	{
		if (sit->getOff() == 1)
		{
			std::cout << "OOOJ" << std::endl;
			out.push_back(pos);
			pos++;
			sit++;
			continue ;
		}
		if (it->revents & (POLLHUP | POLLERR | POLLNVAL))
		{
			std::cerr << "client hang, quiting this client" << std::endl;
			out.push_back(pos);
		}
		else if (it->revents & POLLIN)
		{
			int	nread;
			nread = recv(it->fd, this->recData, 512, 0);
			if (nread <= 0)
			{
				std::cerr << "error at reading, quiting this client" << std::endl;
				out.push_back(pos);
			}
			this->recData[nread] = '\0';
			std::cout << "MSG" << std::endl;
			std::cout << recData << std::endl;
			sit->setMsg(this->recData);
		}
		else if (it->revents & POLLOUT)
		{
			std::vector<std::string>	responses = sit->getResponses();
			std::vector<int>		ncmd = sit->getNCmd();
			std::vector<int>::iterator	ncmdit = ncmd.begin();

			for(std::vector<std::string>::iterator r = responses.begin(); r != responses.end(); r++)
			{
				std::cout << "RESPONSE" << std::endl;
				std::cout << *r << std::endl;
				send(sit->getSocket(), r->c_str(), r->size(), 0);
				/*if (*ncmdit == BAD_PSSWD) //|| *ncmdit == QUIT)
				{
					std::cout << "OOOJ" << std::endl;
					out.push_back(pos);
				}*/
				ncmdit++;
				//pos++;
			}
			sit->emptyResponse();
			sit->emptyNCmd();
			sit->setPollOut(0);
		}
		sit++;
		pos++;
	}
	std::vector<Client>::iterator cl = clients.begin();
	std::vector<struct pollfd>::iterator pfd = clientSock.begin();
	for (std::vector<int>::iterator it = out.begin(); it != out.end(); it++)
	{
		std::cout << "GULIII" << std::endl;
		clientSock.erase(pfd + ((*it) + 1));
		clients.erase(cl + (*it));
	}
}

void	Server::mark(Client *c, std::string msg)//, std::map<int, std::vector<std::string> > cm)
{
	Command cd(c, this->getClients(), hostName, msg, psswd, this->getChannels());
	

	//c->printShait();
	cd.handleCmd();
	//cd.printShait();
}

void	Server::handleMessages()
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getMsg() != "")
		{
			this->mark(&(*it), it->getMsg());
			it->setMsg("");
		}
	}
}
			
void	Server::pollout(int ref)
{
	//POSIBLE SEGFAULT
	if (clientSock.size() <= 1)
		return ;
	std::vector<struct pollfd>::iterator csit = clientSock.begin() + 1;
	if (ref == 1)
	{
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (it->getPollOut() == 1)
				csit->events = (POLLIN | POLLOUT);
			csit++;
		}
	}
	if (ref == 0)
	{
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (it->getPollOut() == 0)
				csit->events = (POLLIN);
			csit++;
		}
	}
}


bool	Server::handleConnections()
{
	while (1)
	{
		pollout(1);
		if (poll(clientSock.data(), clientSock.size(), -1) < 0)
		{
			std::cerr << "poll() error" << std::endl;
		}
		if (clientSock.at(0).revents & POLLIN)
		{
			establishConnection();
		}
		checkClientEvents();
		handleMessages();
		std::cout << "SIZE:" << std::endl;
		std::cout << clientSock.size() << std::endl;
		pollout(0);
	}
}

Server::~Server()
{
}


