#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


class Client
{
	protected:
		int	 	fd;
		int		off;
		int		id;
		int		parseStatus;
		std::string	msg;
		//std::vector<std::string>	params;
		std::map<int, std::vector<std::string> >	cmd;
		std::string	psswd;
		std::string	servPsswd;
		std::string	nickname;
		std::string	username;
 	public:
		Client(int fd, int i, std::string sp);
		void	setMsg(std::string message);
		void	setUser(std::string u) { this->username = u;};
		void	setPsswd(std::string p);
		void	setNick(std::string n);
		int	parseMsg();
		int	getParseStatus();
		int	getOff() { return this->off;};
		std::string getMsg() { return this->msg;};
		std::string getUser() { return this->username;};
		int getId() { return this->id;};
		//std::vector<std::string>	getParams() { return this->params; };
		std::string getNick() { return this->nickname;};
		int	getSocket() { return this->fd;};
    		Client();
    		~Client();
		int	cmdAnalyzer(std::string cmd);
		std::vector<std::string>	split(std::string, const char *c);
		void	launchAction(std::vector<std::string> params);
		//void	rmParams() { params.empty();};
		int	pass(std::vector<std::string> params);
		int	nick(std::vector<std::string> params);
		int	user(std::vector<std::string> params);
		void	notEnoughParams(std::vector<std::string> params);
		void	quit();
		void	handleCmd();
};

#endif
