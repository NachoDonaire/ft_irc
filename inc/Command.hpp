#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Client.hpp>

class Command
{
	private:
		std::string					hostName;
		std::vector<Client *>				clients;
		std::string					msg;
		std::string					servPsswd;
		std::map<int, std::vector<std::string> >	cmd;
		Client						*launcher;
 	public:
    		Command(Client *l, std::vector<Client *> cl, std::string hostN, std::string ms, std::string sp, std::map<int, std::vector<std::string> > cm);
    		~Command();
    		Command();
		Command& operator=(const Command&);
		int	cmdAnalyzer(std::string cmd);
		std::vector<std::string>	split(std::string, const char *c);
		void	markAction(std::vector<std::string> params);
		std::map<int, std::vector<std::string> >	getCmd() { return this->cmd;};
		int	pass(std::vector<std::string> params);
		void	cap(std::vector<std::string> params);
		int	nick(std::vector<std::string> params);
		int	user(std::vector<std::string> params);
		void	notEnoughParams(std::vector<std::string> params);
		void	quit(std::vector<std::string> params);
		void	handleCmd();
		std::string msgGenerator(int msg, std::vector<std::string> params);
		int	welcome(std::vector<std::string> params);
		int	parseMsg();
		void	privmsg(std::vector<std::string> params);
		void	markie(Client *c, std::vector<std::string> params);
		void	markPollOut(std::vector<Client *> clients, std::vector<std::string> dest, std::vector<std::string> params);

};

#endif
