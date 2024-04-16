#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <iostream>

class Client
{
	protected:
		int	 	fd;
		std::string	msg;
		std::vector<std::string>	params;
		std::string	psswd;
		std::string	servPsswd;
		std::string	nick;
 	public:
		Client(int fd, std::string psswdServ);
		void	setMsg(std::string message);
		void	setPsswd(std::string p);
		void	setNick(std::string n);
		int	parseMsg();
		std::string getMsg() { return this->msg;};
		std::vector<std::string>	getParams() { return this->params; };
		std::string getNick() { return this->nick;};
		int	getSocket() { return this->fd;};
    		Client();
    		~Client();
		int	cmdAnalyzer(std::string cmd);
		std::vector<std::string>	split(std::string, char c);
		void						launchAction();
		int							pass();
};

#endif
