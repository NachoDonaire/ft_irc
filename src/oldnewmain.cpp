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

#define CLIENTS 10

void	initializeClients(struct pollfd *csock, int ssock)
{
	for (int i = 0; i < CLIENTS; i++)
	{
		if (i == 0)
			csock[i].fd = ssock;
		csock[i].events = POLLIN;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Choose port" << std::endl;
		return 0;
	}
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int	sskt, nread;
	struct pollfd *csock;
	char	buf[500];

	csock = (struct pollfd *)malloc(sizeof(struct pollfd) * (CLIENTS + 1));
	hints.ai_family = AF_INET; //ipv4
	hints.ai_socktype = SOCK_STREAM; /* tcp */
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;          /* Any protocol */
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;
	
	if (getaddrinfo(NULL, argv[1], &hints, &result) != 0)
		std::cout << "getaddrinfo error" << std::endl;
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sskt = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sskt == -1)
			continue;
		if (bind(sskt, rp->ai_addr, rp->ai_addrlen) == 0)
			break ;
	}
	freeaddrinfo(result);
	if (rp == NULL)
	{
		std::cout << "cannot bind" << std::endl;
	}
	//initializeClients(csock, sskt);
	csock[0].fd = sskt;
	csock[0].events = POLLIN;
	listen(sskt, CLIENTS);
	int	i = 1;
	while (1)
	{
		if (poll(csock, CLIENTS + 1, 50000) < 0)
		{
			std::cout << "poll error" << std::endl;
			return 0;
		}
		if (csock[0].revents & POLLIN)
		{
			csock[i].fd = accept(sskt, rp->ai_addr, &rp->ai_addrlen);
			csock[i].events = POLLIN;
			i++;
		}
		for (int y = 1; y < CLIENTS; y++)
		{
			if (csock[y].revents & POLLIN)
			{
				nread = recv(csock[y].fd, buf, 500, 0);
				if (nread == -1)
					continue ;
				std::cout << "Received data" << std::endl;
				buf[nread] = '\0';
				std::cout << buf << std::endl;
				char bb[2];
				sprintf(bb, "%d", i);
				std::string toSend(bb);
				toSend += " , we got this";
				if (send(csock[y].fd, toSend.c_str(), 15, 0) != 15)
					std::cout << "error sending" << std::endl;
				close(csock[y].fd);
			}
		}
		//close (csock);
	}
	free(csock);
}





