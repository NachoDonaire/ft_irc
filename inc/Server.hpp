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
#include <poll.h>
#include <stdlib.h>



class Server
{
	private:
		char		*port;
		std::string	psswd;
		int		nClientsOnline;
		//harcodeo del tamaño del mensaje recibido
		char 		recData[500];
		struct addrinfo	*rp;
		struct pollfd	*clientSock;
		int		serverSock;
 	public:
    		Server(char *port, char *passwd);
    		Server();
    		~Server();
		bool		handleConnections();
		bool		launchServer();
		//deprecated
		int		lookForAvailableSocket();
		int		reallocPollFd(int index);

};

#endif