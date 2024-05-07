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
#include <Command.hpp>


class Server
{
	private:
		char		*port;
		std::string	psswd;
		std::string	hostName;
		int		nClientsOnline;
		//harcodeo del tamaño del mensaje recibido
		char 		recData[512];
		struct addrinfo	*rp;
		struct pollfd	*clientSock;
		int		serverSock;
		std::vector<Client>	clients;
		Command			cmd;
		//std::map<std::string, std::function<void()> > commands;
 	public:
    		Server(char *port, char *passwd, std::string hostname);
    		Server();
    		~Server();
		bool		handleConnections();
		std::string	msgGenerator(int msg, std::vector<std::string> params, Client c);
		bool		launchServer();
		//deprecated
		int		lookForAvailableSocket();
		int		reallocPollFd(int index);
		void		establishConnection();
		void		checkClientEvents();
		void		handleMessages();
		void		handleError(int status, Client c);
		void		mark(Client *c, std::string msg, std::map<int, std::vector<std::string> > cm);
		void		pass(Client c) const;
		void		pollfdcpy(struct pollfd *src, int size);
		void		printpfd(struct pollfd *src, int size);

};

#endif
