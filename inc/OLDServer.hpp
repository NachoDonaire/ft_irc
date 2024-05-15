#ifndef SERVER_HPP
#define SERVER_HPP

#include <Ircsrv.hpp>
#include <Command.hpp>
#include <Channel.hpp>


class Server
{
	private:
		char		*port;
		std::string	psswd;
		std::string	hostName;
		int		currentSize;
		char 		recData[512];
		struct addrinfo	*rp;
		struct pollfd	*clientSock;
		int		serverSock;
		std::vector<Client>	clients;
		//Command			cmd;
		/* Channel
		std::map<std::string, Channel> channels;
		*/
 	public:
		/* Channel
			createChannel
			deleteChannel ? look in the rfc if it's necessary
		*/
    		Server(char *port, char *passwd, std::string hostname);
    		Server();
    		~Server();
		bool		handleConnections();
		std::string	msgGenerator(int msg, std::vector<std::string> params, Client c);
		bool		launchServer();
		int		pollfdLen();
		int		reallocPollFd(int index);
		void		establishConnection();
		void		checkClientEvents();
		void		handleMessages();
		void		handleError(int status, Client c);
		void		mark(Client *c, std::string msg, std::map<int, std::vector<std::string> > cm);
		void		pass(Client c) const;
		void		pollfdcpy(struct pollfd *src, int size);
		void		printpfd(struct pollfd *src, int size);
		std::vector<Client>		*getClients() { return &this->clients; };

};

#endif
