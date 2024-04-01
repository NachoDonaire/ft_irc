#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>

#define BUF_SIZE 500

void	printAddrInfo(struct addrinfo *r)
{
	struct addrinfo *ptr;
	for (ptr = r; ptr != NULL ; ptr = ptr->ai_next)
	{
		printf("ai_flags: %d\n", ptr->ai_flags);
		printf("ai_family: %d\n", ptr->ai_family);
		printf("ai_socktype: %d\n", ptr->ai_socktype);
		printf("ai_socktype: %d\n", ptr->ai_protocol);
		printf("ai_canonname: %s\n", ptr->ai_canonname);
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	//struct sockaddr_storage peer_addr;
	//socklen_t peer_addr_len;
	ssize_t nread;
	char buf[BUF_SIZE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(hints));
	//af_inet es para ipv4
	hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	//sockstream indica el protocolo tcp
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	s = getaddrinfo(NULL, argv[1], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	//printAddrInfo(result);
	//return (0);
	/* getaddrinfo() returns a list of address structures.
	   Try each address until we successfully bind(2).
	   If socket(2) (or bind(2)) fails, we (close the socket
	   and) try the next address. */

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
		{
			std::cout << rp->ai_addr << std::endl;
			break;                  /* Success */
		}
		close(sfd);
	}

	freeaddrinfo(result);           /* No longer needed */

	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
	listen(sfd, 1);
	/*struct pollfd clientsfd[2];

	clientsfd[0].fd = accept(sfd, NULL, NULL);
	clientsfd[0].events = POLLIN;
	clientsfd[1].fd = accept(sfd, NULL, NULL);
	clientsfd[2].events = POLLIN;
	*/
	for (;;) {
		nread = recv(clientsfd, buf, BUF_SIZE, 0);
		if (nread == -1)
		{
			continue;               /* Ignore failed request */
		}
		std::cout << "RECEIVED DATA" << std::endl;
		std::cout << buf << std::endl;
		if (send(clientsfd, "aloooocote", nread, 0) != nread)
			fprintf(stderr, "Error sending response\n");
		close(clientsfd);
	}
}


