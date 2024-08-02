#include <Server.hpp>
#include <cerrno>
#include <stdio.h>

#define POLLFD_LIMIT -7
int SERVER_ON = 1;

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
		perror("bind");

		close(serverSock);
		return 0;
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

void	Server::treatRecData(int nread)
{
	std::string data(recData);
	
	if (data.size() >= 510)
	{
		std::cout << "GEPASSSSSSSSSSSSSA --->" << data.size() << std::endl;
		//recData[data.size() + 2] = '\0';
		//recData[data.size() + 1] = '\n';
		//recData[data.size()] = '\r';
		return ;
	}
	if (data.find("\r\n") == std::string::npos)
		return ;
	int	i;

	i = nread - 3;
	while (this->recData[i] == ' ')
	{
		i--;
	}
	this->recData[i+1] = '\r';
	this->recData[i+2] = '\n';
	this->recData[i+3] = '\0';
}

void handle_sigint(int sig) 
{
    printf("SIGINT (Ctrl+C) capturado\n");
    SERVER_ON = 0;
    //close(server_socket);
    //exit(0);
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
		std::cout << "SIZE RESPONSEN " << std::endl;
		std::cout << sit->getResponses().size()  << std::endl;
		std::cout << it->events << std::endl;
		std::cout << it->revents << std::endl;
		std::cout << (it->revents & (POLLHUP | POLLERR | POLLNVAL)) << std::endl;
		if (sit->getOff() == 1)
		{
			std::cout << "OOOJ" << std::endl;
			out.push_back(pos);
			//pos++;
			//sit++;
			//continue ;
		}
		if (it->revents & (POLLHUP | POLLERR | POLLNVAL))
		{
			std::cerr << "client hang, quiting this client" << std::endl;
			if (!sit->getOff())
				out.push_back(pos);
		}
		if (it->revents & POLLIN)
		{
			int	nread;
			nread = recv(it->fd, this->recData, 510, 0);
			if (nread <= 0)
			{
				std::cerr << "error at reading, quiting this client" << std::endl;
				if (!sit->getOff())
					out.push_back(pos);
			}
			this->recData[nread] = '\0';
			//std::cout << recData << std::endl;
			for (int i = 0; i < nread; i++)
				write(1, &recData[i], 1);
			treatRecData(nread);
			std::cout << "MSG ---> " << nread <<std::endl;
			for (int i = 0; i < nread; i++)
				write(1, &recData[i], 1);
			//std::cout << recData << std::endl;
			sit->setMsg(this->recData);
			std::cout << sit->getMsg().size() << std::endl;
		}
		if (it->revents & POLLOUT)
		{
			std::cout << "WEEEEEEEE" << std::endl;
			std::vector<std::string>	responses = sit->getResponses();
			std::vector<int>		ncmd = sit->getNCmd();
			std::vector<int>::iterator	ncmdit = ncmd.begin();
			std::cout << "WEEEEEEEE" << std::endl;
			std::cout << sit->getResponses().size()  << std::endl;

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
		std::cout << out.size() << std::endl;
		close(clientSock.at(*it + 1).fd);

		clientSock.erase(pfd + ((*it) + 1));
		std::vector<Client>::iterator cla = clients.begin() + *(it);

		std::cout << cla->getNick() << std::endl;
		
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
		if (it->getMsg() != "" && it->getMsg().find("\r\n") != std::string::npos)
		{
			std::cout << "serious ?????" << std::endl;
			std::cout << it->getMsg().find("\r\n") << std::endl;
			std::cout << it->getMsg() << std::endl;
			this->mark(&(*it), it->getMsg());
			it->setMsg(std::string(""));
			/*if (!it->getAuxMsg().empty())
			{
				std::string validMsg;
				if (it->getAuxMsg().size() > 512)
				{
					validMsg = it->getAuxMsg().substr(0, 512);
					it->getAuxMsg().erase(it->getAuxMsg().begin(), it->getAuxMsg().begin() + 512);
				}
				else
				{
					validMsg = it->getAuxMsg();
					it->getAuxMsg().clear();
				}
				it->setMsg(validMsg);
			}*/
		}
		std::cout << "holaaa !!!!" << std::endl;
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
			{
				std::cout << "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP" << std::endl;
				csit->events = (POLLIN | POLLOUT);
			}
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
	signal(SIGINT, handle_sigint);
	while (SERVER_ON)
	{
		pollout(1);
		if (poll(clientSock.data(), clientSock.size(), -1) < 0)
		{
			std::cerr << "poll() error" << std::endl;
			//return 1;
		}
		if (clientSock.at(0).revents & POLLIN)
		{
			establishConnection();
		}
		/*if (clientSock.at(0).revents & (POLLNVAL))
		{
			//std::cout << clientSock.at(0).fd << std::endl;
		//	std::cout << "JIIISAS" << std::endl;
		}*/
		checkClientEvents();
		handleMessages();
		std::cout << "SIZE:" << std::endl;
		std::cout << clientSock.size() << std::endl;
		pollout(0);
	}
	std::cout << "clientSock SIZE" << clientSock.size() << std::endl;
	//for (std::vector<struct pollfd>::iterator it = clientSock.begin(); it != clientSock.end(); it++)
	for (size_t i = 0; i < clientSock.size(); i++)
	{
		std::cout << "uno" << clientSock.at(i).fd  <<std::endl;
		if (i == 1)
			send(clientSock.at(i).fd, "weba", 4, 0);
		if (close(clientSock.at(i).fd) != 0)
			perror("WOOOOOW FALLA EL CLOSE");
	}
	clientSock.clear();
	return 0;
}

Server::~Server()
{
}


