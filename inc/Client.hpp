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

enum status {
	ERROR = -1,
	REGISTER_PENDING,
	REGISTERED
};

enum command {
	CMD_NOTFOUND,
	NICK_OK,
	NICK_REPEATED,
	WELCOME,
	QUIT,
	CAP_LS,
	CAP_END
};


class Client
{
	protected:
		int	 	fd;
		int	 	command;
		int		off;
		int		registered;
		int		status;
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
		//std::map<int, std::vector<std::string> >	params;
		std::vector<std::string> responses;
 	public:
		Client(int fd, int i, std::string sp, std::string hn);
		void	setMsg(std::string message);
		void	setResponse(std::string r) { this->responses.push_back(r);};
		void	printeito();
		void	setUser(std::string u) { this->username = u;};
		void	setRegister(int i) { this->registered = i;};
		void	setCommand(int i) { this->command = i;};
		void	setPsswd(std::string p);
		void	setParams(std::map<int, std::vector<std::string> > p);
		void	setNick(std::string n);
		void	setOff(int n) { this->off = n;};
		void	setStatus(int n) { this->status = n;};
		void	setPollOut(bool n) { this->pollout = n;};
		int	parseMsg();
		int	getParseStatus();
		int	getStatus() { return this->status ;};
		int	getCommand() { return this->command ;};
		int	getPollOut() { return this->pollout;};
		std::map<int, std::vector<std::string> > getCmd() {return this->cmd;};
		int	getRegister() { return this->registered; };
		int	getOff() { return this->off;};
		void	printCmd();
		std::string getPsswd() { return this->psswd;};
		std::string getMsg() { return this->msg;};
		std::string getUser() { return this->username;};
		int getId() { return this->id;};
		//std::map<int, std::vector<std::string> >	getParams() { return this->params; };
		std::string getNick() { return this->nickname;};
		int	getSocket() { return this->fd;};
		std::vector<std::string> getCommands() { return this->commands;};
		void	emptyResponse() { this->responses.empty(); };
		std::vector<std::string>	split(std::string na, const char *c);
		std::vector<std::string>	getResponses() { return this->responses; };
    		Client();
    		~Client();
		//void	launchAction(std::vector<std::string> params);
		void	handleCmd();
};

#endif
