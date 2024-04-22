#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <stdlib.h>
#include <Client.hpp>


class Server
{
	private:
		char		*port;
		std::string	psswd;
		int		nClientsOnline;
		//harcodeo del tamaño del mensaje recibido
		char 		recData[512];
		struct addrinfo	*rp;
		struct pollfd	*clientSock;
		int		serverSock;
		std::vector<Client>	clients;
		//std::map<std::string, std::function<void()> > commands;
 	public:
    		Server(char *port, char *passwd);
    		Server();
    		~Server();
		bool		handleConnections();
		bool		launchServer();
		//deprecated
		int		lookForAvailableSocket();
		int		reallocPollFd(int index);
		void		establishConnection();
		void		checkClientEvents();
		void		handleMessages();
		void		handleError(int status, Client c);
		void		launchAction(Client c);
		void		pass(Client c) const;
		void		pollfdcpy(struct pollfd *src, int size);
		void		printpfd(struct pollfd *src, int size);

};

#endif
