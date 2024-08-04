#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Client.hpp>
#include <set>
#include <Ircsrv.hpp>
#include <Channel.hpp>
typedef std::vector<Channel>	vectorCh;
typedef std::string		str;

class Command
{
	private:
		std::string					hostName;
		std::string					optOff;
		std::vector<std::string>			argOff;
		std::vector<Client >				*clients;
		std::string					msg;
		std::string					servPsswd;
		std::map<int, std::vector<std::string> >	cmd;
		Client						*launcher;
		/* Channel */
		vectorCh					*channels;
 	public:
    		Command(Client *l, std::vector<Client> *cl, std::string hostN, std::string ms, std::string sp, vectorCh* serverChannels);
    		~Command();
    		Command();
		Command& operator=(const Command&);
		std::vector<std::string>	split(std::string, const char *c);
		std::vector<std::string>	split(std::string, const char c);
		void	markAction(std::vector<std::string> params);
		std::map<int, std::vector<std::string> >	getCmd() { return this->cmd;};
		int	pass(std::vector<std::string> params);
		void	extractFlag(size_t pos);
		bool	oFlagChecker(std::string option);
		void	cap(std::vector<std::string> params);
		void	topic(std::vector<std::string> params);
		void	who(std::vector<std::string> params);
		void	invite(std::vector<std::string> params);
		int	nick(std::vector<std::string> params);
		int	parseNick(std::string);
		int	user(std::vector<std::string> params);
		void	markChannel(Channel *ch, std::vector<std::string> params);
		void	modeOptions(Channel *ch, std::vector<std::string> params);
		void	modeInstructions(Channel *ch, std::vector<std::string> params);
		int	plusMode(Channel *ch, std::vector<std::string> params, char opt, char sign, size_t pos);
		std::string options(std::vector<std::string> params);
		bool	isModeOption(char c);
		void	quit(std::vector<std::string> params);
		void	part(std::vector<std::string> params);
		void	handleCmd();
		Channel	*getChannelByName(std::string name);
		void	kick(std::vector<std::string>& params);
		void	kickProcessOne(const std::vector<std::string>& params);
		void	mode(std::vector<std::string> params);
		std::string	rmErrorFlags(std::string opt, std::vector<int> rm);
		std::vector<std::string>	rmErrorArgs(std::vector<std::string> params, std::vector<int> rm, size_t p);
		void	msgError(int type, std::vector<std::string> params);
		void	notRegistered(std::vector<std::string> nick);
		std::string responseGenerator(int msg, std::vector<std::string> params);
		int	welcome(std::vector<std::string> params);
		int	parseMsg();
		void	printShait();
		void	privmsg(std::vector<std::string> params);
		void	markie(Client *c, std::vector<std::string> params, int cmd);
		void	markEverything(int cmd, std::vector<std::string> params);
		void	markEveryThing(str& cmd, vectorStr& params);
		void	markClientsPollOut(std::vector<Client> *clients, std::vector<std::string> dest, std::vector<std::string> params, int type);
		void	markChannelPollOut(std::vector<Channel> *clients, std::vector<std::string> dest, std::vector<std::string> params, int type);
		/* Channel */
		void				join(const std::vector<std::string>& params, str &userId, std::string chaName, size_t pos);
		vectorCh::iterator		getChannel(const str& id);
		int				stringToEnum(const str& str);
		Client				*findClientByNick(std::string Nick);
		void				popAllCoincidences(std::vector<std::string> *vec, std::string c);
		//std::vector<std::string>	getChannelNicks(Channel ch);
};

#endif
