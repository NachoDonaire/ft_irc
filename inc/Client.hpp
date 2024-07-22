#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <Ircsrv.hpp>
typedef std::vector<std::string *> 	vectorStr;
typedef std::string 			str;

enum status {
	ERROR = -1,
	REGISTER_PENDING,
	REGISTERED
};

enum command {
	CMD_ERROR = -1,
	CMD_NOTFOUND,
	NICK_OK,
	NICK_REPEATED,
	WELCOME,
	QUIT,
	PART,
	CAP_LS,
	CAP_END,
	PRIVMSG,
	BAD_PSSWD,
	USER,
	OK_PSSWD,
	JOIN_OK,
	BLANK,
	WHO_BEGIN,
	WHO_END,
	ERR_NEEDMOREPARAMS,
	ERR_INVITEONLYCHAN,
	ERR_CHANNELISFULL,
	ERR_NOSUCHCHANNEL,
	ERR_CHANOPRIVSNEEDED,
	ERR_NOTONCHANNEL,
	ERR_BADCHANNELKEY,
	ERR_ERRONEUSNICKNAME,
	ERR_BADCHANNELID,
	ERR_USERNOTINCHANNEL,
	RPL_TOPIC,
	RPL_NAMREPLY,
	NOT_REGISTERED,
	KICK
};


class Client
{
	private:
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
		std::vector<int>	 cmds;

		// Channel
		vectorStr	channels;
		
 	public:
		// Channel
		void	join(const str& channelId);
		void	kick(const str& channelId);
		vectorStr	getChannels() const;
		void		setChannels(const vectorStr& newChannels);

		Client(int fd, int i, std::string sp, std::string hn);
		void	setMsg(std::string message);
		void	setResponse(std::string r) { this->responses.push_back(r);};
		void	printeito();
		void	setUser(std::string u) { this->username = u;};
		void	setRegister(int i) { this->registered = i;};
		void	setNCmd(int i) { this->cmds.push_back(i);};
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
		//std::string *getNickPtr() { return this->nickname;};
		std::string &getNick() { return (this->nickname);};
		int	getSocket() { return this->fd;};
		std::vector<std::string> getCommands() { return this->commands;};
		void	emptyResponse() { this->responses.clear(); };
		void	emptyCmd() { this->cmd.clear(); };
		void	emptyNCmd() { this->cmds.clear(); };
		void	printShait() ;
		std::vector<std::string>	split(std::string na, const char *c);
		std::vector<std::string>	getResponses() { return this->responses; };
		std::vector<int>	getNCmd() { return this->cmds; };
    		Client();
    		~Client();
		//void	launchAction(std::vector<std::string> params);
		void	handleCmd();
		Client* operator=(const Client* f);
};

#endif
