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
//#include <Channel.hpp>


class Client
{
	protected:
		int	 	fd;
		int		off;
		int		registered;
		bool		pollout;
		int		id;
		std::vector<std::string>	commands;
		int		parseStatus;
		std::string	msg;
		//std::vector<std::string>	params;
		std::map<int, std::vector<std::string> >	cmd;
		std::string	psswd;
		std::string	servPsswd;
		std::string	hostName;
		std::string	nickname;
		std::string	username;
		//std::vector<Channel> channels;
 	public:
		Client(int fd, int i, std::string sp, std::string hn);
		void	setMsg(std::string message);
		void	printeito();
		void	setUser(std::string u) { this->username = u;};
		void	setRegister(int i) { this->registered = i;};
		void	cap(std::vector<std::string> params);
		void	setPsswd(std::string p);
		void	setNick(std::string n);
		void	setOff(int n) { this->off = n;};
		void	setPollOut(bool n) { this->pollout = n;};
		int	parseMsg();
		int	getParseStatus();
		int	getPollOut() { return this->pollout;};
		std::map<int, std::vector<std::string> > getCmd() {return this->cmd;};
		int	getRegister() { return this->registered; };
		int	getOff() { return this->off;};
		std::string getPsswd() { return this->psswd;};
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
		void	quit(std::vector<std::string> params);
		void	handleCmd();
		int	welcome();
		std::string	msgGenerator(int msg, std::vector<std::string> params);
};

#endif
