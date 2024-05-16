#ifndef SERVER_HPP
#define SERVER_HPP

#include <Ircsrv.hpp>
#include <Command.hpp>
#include <Channel.hpp>


class Server
{
	private:
		std::string	port;
		std::string	psswd;
		std::string	hostName;
		int		currentSize;
		char 		recData[512];
		struct addrinfo	*rp;
		std::vector<struct pollfd>	clientSock;
		int		serverSock;
		std::vector<Client>	clients;
 	public:
    		Server(char *port, char *passwd, std::string hostname);
    		Server();
    		~Server();
		bool		launchServer();
		void		reallocPlus();
		void		pollout(int ref);
		void		establishConnection();
		void		checkClientEvents();
		void		mark(Client *c, std::string msg);
		void		handleMessages();
		void		printClientSock();
		bool		handleConnections();
		std::vector<Client>		*getClients() { return &this->clients; };

};

#endif
