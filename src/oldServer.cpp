#include <Server.hpp>

Server::Server()
{
}

Server::Server(char *p, char *pd) : port(p)
{
	psswd = std::string(pd);
	//superpivisional el malloc, aunque sospecho que no queda otra
	this->clientSock = (struct pollfd *)malloc(sizeof(pollfd) * 5);
	//again, harcodeo del nclients
	for (int i = 0; i < 5; i++)
	{
		clientSock[i].fd = -1;
	}
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

bool	Server::handleConnections()
{
	bool	error = 0;
	while (error == 0)
	{
		//harcodeo de nclientes + server sockets y el timeout
		if (poll(clientSock, 6, 50000) < 0)
		{
			std::cerr << "poll() error" << std::endl;
			return 1;
		}
		if (clientSock[0].revents & POLLIN)
		{
			std::cout << "Someones connecting" << std::endl;
			int	pos = this->lookForAvailableSocket();
			if (pos == -1)
			{
				std::cerr << "Not available client socket" << std::endl;
			}
			clientSock[pos].fd = accept(serverSock, rp->ai_addr, &rp->ai_addrlen);
			clientSock[pos].events = POLLIN;
		}
		//harcodeo again de n clientes
		for (int y = 1; y < 6; y++)
		{
			if (clientSock[y].revents & POLLIN && clientSock[y].fd != -1)
			{
				//harcodeando again | tamaño del mensaje
				int nread = recv(clientSock[y].fd, this->recData, 500, 0);
				this->recData[nread] = '\0';
				std::cout << "In the name of the: " << y << std::endl;
				send(clientSock[y].fd, "response", 8, 0);
				std::cout << recData << std::endl;
				//close(clientSock[y].fd);
			}
		}
			//close socket ??
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


