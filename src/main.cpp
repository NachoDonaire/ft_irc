#include <Server.hpp>

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
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << std::endl;
		std::cerr << "1. port\n2. password" << std::endl;
		return 1;
	}
	Server	irc(argv[1], argv[2]);

	if (irc.launchServer() != 0)
		return 1;
	return 0;

}





