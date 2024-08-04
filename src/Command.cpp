#include <Command.hpp>

Command::Command()
{}

Command::Command(Client *l, std::vector<Client> *cl, std::string hostN, std::string ms, std::string sp, vectorCh* serverChannels)
{

	msg = ms;
	this->parseMsg();
	clients = cl;
	servPsswd = sp;
	hostName = hostN;
	launcher = l;
	channels = serverChannels;
	//std::cout << "pero toocho" << std::endl;
}

//printea informacion relativa a la clase. launcher es el cliente que lanza el comando y tiene una funcion analoga
void	Command::printShait()
{
	std::cout << "-------------------" << std::endl;
	std::cout << hostName << std::endl;
	std::cout << "msg : " << msg << std::endl;
	std::cout << servPsswd << std::endl;
	std::cout << "Commands : " << std::endl;
	for (std::map<int, std::vector<std::string> >::iterator it = cmd.begin(); it != cmd.end(); it++)
	{
		std::cout << "this is command cmd : " << std::endl;
		for (size_t i = 0; i < it->second.size() ; i++)
		{
			std::cout << it->second.at(i) << std::endl;
		}
	}
	std::cout << "############" << std::endl;
	std::cout << "launcher : " << std::endl;
	launcher->printShait();
	std::cout << "############" << std::endl;
	std::cout << "-------------------" << std::endl;
}

	

std::string	Command::responseGenerator(int msg, std::vector<std::string> params)
{
	std::string response(":");
	response += hostName;

	switch (msg)
	{
		case(CMD_NOTFOUND):
			response += " 421 " + params[0] + " :Unknown command";
			break;
		case(OK_PSSWD):
			response += "";
			break;
		case(INVITE):
			response += " 341 " + launcher->getNick() + " " + params[1] + " " + params[2];

			break;
		case(TOPIC_OK):{
			Channel *ch = getChannelByName(params[1]);
			response += " 332 " + launcher->getNick() + " " + params[1] + " " + ch->getTopic();
			break;}
		case(NOTOPIC):
			response += " 331 " + launcher->getNick() + " " + params[1] + " :No topic is set";
			break;
		case(MODE_OK):
			response = ":@" + launcher->getNick() + " MODE";
			for (size_t i = 1; i < params.size(); i++)
			{
				response += " ";
				response += params.at(i);
			}
			break;
		case(KICK):
			response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " KICK " + params[1] + " " + params[2];
			if (params.size() > 3)
			{
				for (std::vector<std::string>::iterator it = params.begin() + 3; it != params.end(); it++)
				{
					response += " ";
					response += *it;
				}
			}
			break;
		case(PART):
			response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " PART " + params[1];
			if (params.size() > 3)
			{
				for (std::vector<std::string>::iterator it = params.begin() + 3; it != params.end(); it++)
				{
					response += " ";
					response += *it;
				}
			}
			break;
		case(MODE_WITH_CHANNEL):
			response += " 324 " + launcher->getNick() + " " + params.at(1);
			for (size_t i = 2; i < params.size(); i++)
			{
				response += " ";
				response += params.at(i);
			}
			break;
		case(ERR_NOTONCHANNEL):{
			std::string chname = params.at(params.size() - 1);

			response += " 442 " + launcher->getNick() + " " + chname + " :You are not in this channel";
			break;}
		case(ERR_INVITEONLYCHAN):
			response += " 473 " + launcher->getNick() + " " + params[1]  + " :Cannot join the channel (+i)";
			break;
		case(ERR_UNKNOWNMODE):
			response += " 472 " + launcher->getNick() + " " + params[1] + " " + params[0] + " :Not valid mode";
			break;
		case(NICK_OK):
			if (launcher->getNick() == "")
			{
				launcher->setNick(params[1]);
				launcher->setLogFail(0);
				response += " 381 " + launcher->getNick() + " :Nick " + launcher->getNick() + " magnificent set!";
			}
			else
			{
				response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " NICK :" + params[1];
				launcher->setNick(params[1]);
				launcher->setLogFail(0);
			}
			break;
		case(NICK_REPEATED):
			response += " 433 * " + params[1] + " :Nickname is already in use.";
			break;
		case(ERR_ERRONEUSNICKNAME):
			response += " 432 " + launcher->getUser() + " " + params[1];
			break;
		case(ERR_ALREADYREGISTRED):
			response += " 462 " + launcher->getNick() + " :You already registered";
			break;
		case(ERR_BADCHANNELID):
			response +=  " 476 " + launcher->getNick() +  " " + params[1] + " :Bad Channel name";
			break;
		case(QUIT):
			if (params.size() <= 1)
				response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " QUIT";
			else
			{
				response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " QUIT ";
				for (size_t i = 1; i < params.size(); i++)
				{
					response += " ";
					response += params[i];
				}
			}
			break;
		case(ERR_NEEDMOREPARAMS):
		
			response += " 461 ";
			response += params[0];
			if (params.size() > 1)
			{
				Channel *ch = getChannelByName(params[1]);
				if (ch)
					response += " " + params[1] + " ";
			}
			response += " :Not enough or too much params for ";
			response += params[0];
			break;
		case(ERR_NOSUCHCHANNEL):
			response += " 403 " + launcher->getNick() + " :No such a channel";
			break;
		case(ERR_NOSUCHNICK):
			response += " 401 " + launcher->getNick() + " :" + params[0] + " No such nick";
			break;
		case(ERR_CHANNELISFULL):
			response += " 471 " + launcher->getNick() + " " + params[1] + " :This channel is full";
			break;
		case(ERR_CHANOPRIVSNEEDED):
			response += " 482 " + launcher->getNick() + " " + params[1] +  " :You are not a chop";
			break;
		case(ERR_USERNOTINCHANNEL):
			response += " 441 " + launcher->getNick() + " " + params[1] + " :User not on channel";
			break;
		case(ERR_USERONCHANNEL):
			response += " 443 " + launcher->getNick() + " " + params[2] + " :User already on channel";
			break;
		case(WELCOME):
			response += " 001 " + launcher->getNick() + " :Welcome to ircserv !";
			break;
		case(BAD_PSSWD):
			std::cout << "hooolaaaa" << std::endl;
			response = "ERROR :Error: bad password or missing one. Disconnecting...";
			launcher->setOff(1);
			break;
		case(PRIVMSG):
			std::cout << "EORESP " << params.size()  <<std::endl;
			response = ":";
			if (params[0].find('#') == 0 || params[0].find('&') == 0)
			{	
				Channel *ch = getChannelByName(params[0]);
				if (ch->isAdmin(launcher->getNick()))
					response += "@";
			}
			response += launcher->getNick();
			response += " PRIVMSG " + params[0] +  " ";
			if (params.size() == 1)
				break ;
			for (size_t  i = 1; i < params.size(); i++)
			{
				response += params[i];
				response += " ";
			}
			std::cout << "EORESP" << std::endl;
			break;
		case(WHO_BEGIN):
			response += " 352 " + launcher->getNick() + " " + params[1] + " " +  launcher->getUser() + " " +  hostName + " " +  launcher->getNick() + " H :0 " + launcher->getUser();
											break;
		case(WHO_END):
			response += " 315 " + launcher->getNick() + " " + params[1] + " :End of list";

			break;
		case(JOIN_OK):
			response = ":" + launcher->getNick() + " JOIN " + params[0];
			break;
		case(ERR_BADCHANNELKEY):
			response += " 475 " + launcher->getNick() + " " + params[1] + " :Cannot join channel (";
			break;
		case(NOT_REGISTERED):
			response += " 451 * :You have not registered";
			break;
		case(ERR_BADCHANMASK):
			response += " 476 " + launcher->getNick() + " " + params[1] + " Channel must begin by '#' character";
			break;
/*
		case(RPL_WHOREPLY):
			//
			break;
		case(RPL_ENDOFWHO):
			//
			break;
*/
	}
	return (response + "\r\n");
}



Command& Command::operator=(const Command& f)
{
	clients = f.clients;
	servPsswd = f.servPsswd;
	hostName = f.hostName;
	msg = f.msg;
	launcher = f.launcher;
	cmd = f.cmd;
	return *this;
}



Command::~Command()
{
}

void	Command::handleCmd()
{	
	std::vector<std::string> params;
	for (std::map<int, std::vector<std::string> >::iterator i = cmd.begin(); i != cmd.end(); i++)
	{
		for (std::vector<std::string>::iterator y = i->second.begin(); y != i->second.end(); y++)
		{
			params.push_back(*y);
		}
		markAction(params);
		params.clear();
		if (launcher->getOff() == 1)
			return ;
	}
}

void	Command::markAction(std::vector<std::string> params)
{
	std::string cmd = params[0];

	std::cout << "EVALUATING : " << params[0] << std::endl;
	if (cmd == "PASS")
		pass(params);
	else if (cmd == "NICK")
		nick(params);
	else if (cmd == "USER")
		user(params);
	else if (cmd == "CAP")
		cap(params);
	else if (cmd == "QUIT")
		quit(params);
	else if  (cmd == "PART")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		if (channels->size() == 0)
			return msgError(ERR_NOSUCHCHANNEL, params);
		part(params);
	}
	else if  (cmd == "TOPIC")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		if (channels->size() == 0)
			return msgError(ERR_NOSUCHCHANNEL, params);
		topic(params);
	}

	else if  (cmd == "INVITE")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		if (channels->size() == 0)
			return msgError(ERR_NOSUCHCHANNEL, params);
		invite(params);
	}
	else if (cmd == "PRIVMSG")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		privmsg(params);
	}
	else if (cmd == "KICK")
	{
	std::cout << "WEEEEEEEEBA" << std::endl;
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		std::cout << "WEEEEEEEEBA" << std::endl;
		kick(params);
	}
	else if (cmd == "MODE")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		mode(params);
	}
	else if (cmd == "JOIN")
	{
		std::vector<std::string> channels_name;
		std::vector<std::string> parameters;
		std::vector<std::string> channs;
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		int responseType;
	
		channels_name = split(params[1], ',');
		int	pos = 0;
		for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); it++)
		{
			try
			{
				join(params, launcher->getNick(), *it, pos);
				responseType = JOIN_OK;
				parameters.push_back(*it);
			}
			catch(std::exception& e)
			{
				responseType = stringToEnum(e.what());
				std::cout << e.what() << std::endl;
				params[1] = *it;
			}

			if (!parameters.empty())
			{
				channs.push_back(*it);
				markEverything(responseType, parameters);
				markChannelPollOut(channels, channs, parameters, responseType);
				parameters.clear();
				channs.clear();
			}
			else
				markEverything(responseType, params);
			pos++;
		}
		launcher->setPollOut(1);
		return ;
	}
	else if (cmd == "WHO")
	{
		return ;
	}
	else if (cmd == "")
		return ;
	else
	{
		launcher->setPollOut(1);
		if (!launcher->getRegister())
		{
			notRegistered(params);
			//markEverything(BAD_PSSWD, params);
			return ;
		}
		markEverything(CMD_NOTFOUND, params);
	}
}

void	Command::notRegistered(std::vector<std::string> params)
{
	if (launcher->getPsswd() == "")
		markEverything(BAD_PSSWD, params);
	else
		markEverything(NOT_REGISTERED, params);
	launcher->setPollOut(1);
}

void	Command::who(std::vector<std::string> params)
{
	markEverything(WHO_BEGIN, params);
	markEverything(WHO_END, params);
	launcher->setPollOut(1);
}

void	Command::cap(std::vector<std::string> params)
{
	if (params.size() <= 1)
	{
		markEverything(ERR_NEEDMOREPARAMS, params);
		launcher->setPollOut(1);
		return ;
	}
	if (params[1] == "LS")
		launcher->setCommand(CAP_LS);
	else if (params[1] == "END")
		launcher->setCommand(CAP_END);
	
	markEverything(launcher->getCommand(), params);
}


std::vector<std::string> Command::split(std::string str, const char *delimiter)
{
	std::vector<std::string> result;
	std::string token;
	std::string delimiters(delimiter);
	std::set<char> delimiterSet(delimiters.begin(), delimiters.end());

	for (std::string::iterator ch = str.begin() ; ch != str.end() ; ch++) {
		if (delimiterSet.find(*ch) != delimiterSet.end()) {
			if (!token.empty()) {
				result.push_back(token);
				token.clear();
			}
		} else {
			token += *ch;
		}
	}
	if (!token.empty()) {
		result.push_back(token);
	}
	return result;
}

std::vector<std::string> Command::split(std::string str, const char delimiter)
{
	std::vector<std::string> result;
	std::string token;
	for (std::string::iterator ch = str.begin() ; ch != str.end() ; ch++) {
		if (*ch == delimiter) {
			if (!token.empty()) {
				result.push_back(token);
				token.clear();
			}
		} else {
			token += *ch;
		}
	}
	if (!token.empty()) {
		result.push_back(token);
	}
	return result;
}
//aver que hacer con mas de 512 bytes
int Command::parseMsg()
{
	std::vector<std::string> tokens;
	std::vector<std::string> parameters;

	if (msg.find("\r\n") == std::string::npos)
	{
		return (2);
	}
	tokens = split(msg, "\r\n");
	for (size_t i = 0; i < tokens.size(); i++)
	{
	
		parameters = split(tokens[i], ' ');
		cmd[i] = parameters;
		parameters.clear();
	}
	return (0);
}

void	Command::markEverything(int cmd, std::vector<std::string> params)
{
	std::string response;

	launcher->setCommand(cmd);
	if (cmd == JOIN_OK)
	{
		response = responseGenerator(launcher->getCommand(), params);
		std::string isempty = response.substr(1, response.size());
		if (isempty != hostName + "\r\n")
		{
			launcher->setResponse(response);
			launcher->setNCmd(launcher->getCommand());
		}
		return ;
	}
	response = responseGenerator(launcher->getCommand(), params);
	std::string isempty = response.substr(1, response.size());
	if (isempty != hostName + "\r\n")
	{
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
	}
}

int	Command::pass(std::vector<std::string> params)
{
	std::string response;

	if (params.size() != 2)
	{
		markEverything(ERR_NEEDMOREPARAMS, params);
		launcher->setPollOut(1);
		return 3;
	}
	launcher->setPsswd(params[1]);
	if (launcher->getPsswd() != servPsswd)
	{
		markEverything(BAD_PSSWD, params);
		launcher->setPollOut(1);
		return 3;
	}
	else
		markEverything(OK_PSSWD, params);
	if (launcher->getRegister())
		launcher->setPollOut(1);
	else
		welcome(params);
	return 0;
}

int	Command::user(std::vector<std::string> params)
{
	std::string response;
	if(launcher->getRegister())
	{
		markEverything(ERR_ALREADYREGISTRED, params);
		launcher->setPollOut(1);
		launcher->setLogFail(1);
		return 3;
	}
	if (launcher->getLogFail() == 23)
	{
		notRegistered(params);
		launcher->setLogFail(1);
		return 3;
	}
	if (params.size() < 5)
	{
		markEverything(ERR_NEEDMOREPARAMS, params);
		launcher->setPollOut(1);
		launcher->setLogFail(1);
		return 3;
	}
	launcher->setUser(params[1]);
	launcher->setLogFail(0);
	markEverything(USER, params);
	welcome(params);
	return 0;
}

int	Command::parseNick(std::string nick)
{
	size_t	pos;

	pos = 0;
	while (pos < nick.size())
	{
		if ((nick[pos] >= 'A' && nick[pos] <= '^') || (nick[pos] >= '`' && nick[pos] <= '{') || nick[pos] == '}' || (nick[pos] >= '0' && nick[pos] <= '9') || nick[pos] == '_' || nick[pos] == '-')
			pos++;
		else
			return (0);
	}
	return (1);
}


int	Command::nick(std::vector<std::string> params)
{
	std::string response;

	if (launcher->getPsswd() == "")
	{
		markEverything(BAD_PSSWD, params);
		launcher->setPollOut(1);
		launcher->setLogFail(3);
		return 3;
	}
	if (launcher->getLogFail() == 1)
	{
		markEverything(NOT_REGISTERED, params);
		launcher->setPollOut(1);
		launcher->setLogFail(3);
		return 3;
	}
	if (params.size() != 2)
	{
		markEverything(ERR_NEEDMOREPARAMS, params);
		launcher->setPollOut(1);
		launcher->setLogFail(3);
		return 3;
	}
	if (params[1].size() > 9 || parseNick(params[1]) != 1)
	{
		markEverything(ERR_ERRONEUSNICKNAME, params);
		launcher->setPollOut(1);
		launcher->setLogFail(3);
		return 3;
	}
	for (std::vector<Client >::iterator it = clients->begin(); it != clients->end(); it++)
	{
		if ((*it).getNick() == params[1])
		{
			markEverything(NICK_REPEATED, params);
			launcher->setPollOut(1);
			launcher->setLogFail(3);
			return 2;
		}
	}
	markEverything(NICK_OK, params);
	launcher->setPollOut(1);
	welcome(params);
	return 0;
}

void	Command::popAllCoincidences(std::vector<std::string> *vec, std::string coincidence)
{
	for (std::vector<std::string>::iterator it = vec->begin(); it != vec->end(); it++)
	{
		if (*it == coincidence)
			vec->erase(it);
	}
}

Client	*Command::findClientByNick(std::string nick)
{
	for (std::vector<Client>::iterator cl = clients->begin(); cl != clients->end(); cl++)
	{
		if (cl->getNick() == nick)
			return &(*cl);
	}
	std::cout << "FINDCLIENT" << std::endl; 
	return (&(*clients->end()));
}

void	Command::quit(std::vector<std::string> params)
{
	launcher->setOff(1);
	std::string nick = launcher->getNick();
	std::vector<int>	delPos;
	std::vector<std::string>	partParams;
	std::string			channelsToLeave;

	partParams.push_back("PART");

	std::cout << "QUIT" << std::endl;
	for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); it++)
	{
		it->printAdminUsers();
		if (it->isAdmin(nick) || it->isUser(nick))
			channelsToLeave += it->getId() + std::string(",");
		it->printAdminUsers();
	}
	std::cout << "Channels size when quitting: "<< channels->size() <<std::endl;
	markEverything(QUIT, params);
	if (channelsToLeave.size() > 0)
	{
		channelsToLeave.erase(channelsToLeave.begin() + channelsToLeave.size() - 1);
		partParams.push_back(channelsToLeave);
		part(partParams);
	}
	std::cout << "Channels size when quitting: "<< channels->size() <<std::endl;
	launcher->setPollOut(1);
}

int Command::welcome(std::vector<std::string> params)
{
	std::string response;

	if (launcher->getNick() != "" && launcher->getUser() != "" && launcher->getPsswd() != "" && !launcher->getRegister())
	{
		launcher->setRegister(1);
		markEverything(WELCOME, params);
		launcher->setPollOut(1);
	}
	return (0);
}

void	Command::markie(Client *c, std::vector<std::string> params, int cmd)
{
	std::string response;

	c->setCommand(cmd);
	response = responseGenerator(c->getCommand(), params);
	c->setResponse(response);
	c->setNCmd(c->getCommand());
	c->setPollOut(1);
}

void	Command::markClientsPollOut(std::vector<Client> *clients, std::vector<std::string> dest, std::vector<std::string> params, int type)
{
	std::cout << "BEGIN OF CLIENT" << std::endl;
	for (size_t y = 0; y < dest.size(); y++)
	{
		std::cout << dest[y] << std::endl;
		if (findClientByNick(dest[y]) == &(*clients->end()) && dest[y].find('#') != 0)
		{
			std::vector<std::string> parameters;
			parameters.push_back(dest[y]);
			markEverything(ERR_NOSUCHNICK, parameters);
			parameters.clear();
			launcher->setPollOut(1);
			continue ;
		}
		for (size_t i = 0; i < clients->size(); i++)
		{
			std::cout << "1" << std::endl;
			if (dest[y] == clients->at(i).getNick())
			{
				std::cout << "SHEGA?" << std::endl;
				std::vector<std::string> parameters;
				parameters.push_back(dest[y]);
				for (size_t i = 2; i < params.size(); i++)
					parameters.push_back(params[i]);
				if (type == PRIVMSG)
					markie(&clients->at(i), parameters, type);
				else
					markie(&clients->at(i), params, type);
			}
				
		}
	}
	std::cout << "END OF CLIENT" << std::endl;
}

void	Command::markChannelPollOut(std::vector<Channel> *ch, std::vector<std::string> dest, std::vector<std::string> params, int type)
{
	for (size_t y = 0; y < dest.size(); y++)
	{
		if ((dest[y].find('#') == 0 || dest[y].find('&') == 0) && !getChannelByName(dest[y]))
		{
			markEverything(ERR_NOSUCHCHANNEL, params);
			launcher->setPollOut(1);
			continue ;
		}
		for (size_t i = 0; i < ch->size(); i++)
		{
			if (dest[y] == ch->at(i).getId())
			{
				//if (((!ch->at(i).isUser(dest[y]) && !ch->at(i).isAdmin(dest[y])) || 
				if (((!ch->at(i).isUser(launcher->getNick()) && !ch->at(i).isAdmin(launcher->getNick()))) && type == PRIVMSG)
				{
					std::cout << "PASA POR PRIVMSG SIN SERLO : " << dest[y] << " " << launcher->getNick() << std::endl;
					params.push_back(ch->at(i).getId());
					markEverything(ERR_NOTONCHANNEL, params);
					launcher->setPollOut(1);
					continue ;
				}
				for  (size_t j = 0; j < ch->at(i).getUsers().size(); j++)
				{
					if (*(ch->at(i).getUsers().at(j)) == launcher->getNick())
						continue ;
				
					Client *cl = findClientByNick(*(ch->at(i).getUsers().at(j)));
					if (cl && cl != &(*(clients->end())))
					{
						std::vector<std::string> parameters;
						parameters.push_back(dest[y]);
						for (size_t i = 2; i < params.size(); i++)
							parameters.push_back(params[i]);
						if (type == PRIVMSG)
							markie(cl, parameters, type);
						else
							markie(cl, params, type);
					}
				}
				for  (size_t j = 0; j < ch->at(i).getAdmins().size(); j++)
				{
					if (*(ch->at(i).getAdmins().at(j)) == launcher->getNick())
						continue ;
					Client *cl = findClientByNick(*(ch->at(i).getAdmins().at(j)));
					if (cl && cl != &(*(clients->end())))
					{
						std::vector<std::string> parameters;
						parameters.push_back(dest[y]);
						for (size_t i = 2; i < params.size(); i++)
							parameters.push_back(params[i]);
						if (type == PRIVMSG)
							markie(cl, parameters, type);
						else
							markie(cl, params, type);
					}
				}
			}
		}
	}
}

void	Command::privmsg(std::vector<std::string> params)
{
	std::vector<std::string>	dest;
	std::string			response;

	if (params.size() < 3)
	{
		launcher->setCommand(ERR_NEEDMOREPARAMS);
		response = responseGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
		launcher->setPollOut(1);
		return ;
	}
	dest = split(params[1], ",");
	markClientsPollOut(clients, dest, params, PRIVMSG);
	markChannelPollOut(channels, dest, params, PRIVMSG);
}

Channel *Command::getChannelByName(std::string name)
{
	for (vectorCh::iterator i = channels->begin(); i != channels->end(); i++)
	{
		if (i->getId() == name)
			return &(*i);
	}
	return NULL;
}

void Command::msgError(int type, std::vector<std::string> params)
{
	std::string response;
	
	launcher->setCommand(type);
	response = responseGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
	launcher->setPollOut(1);
	return ;
}

void	Command::markChannel(Channel *ch, std::vector<std::string> params)
{
	//ch->printAdminUsers();
	int	us;
	int	ad;
	int	i;
	
	us = -1;
	ad = -1;
	i = 0;
	std::cout << "KICK REMOVING AND MARKING" << std::endl;
	ch->printAdminUsers();
	for (std::vector<std::string*>::iterator u = ch->getUsers().begin() ; u != ch->getUsers().end(); u++)
	{
		Client *c = findClientByNick(**u);
		if (c && c != &(*(clients->end())))
		{
			markie(c, params, KICK);
			if (c->getNick() == params[2])
				us = i;
		}
		i++;
	}
	i = 0;
	for (std::vector<std::string*>::iterator u = ch->getAdmins().begin(); u != ch->getAdmins().end(); u++)
	{
		Client *c = findClientByNick(**u);
		if (c && c != &(*(clients->end())))
		{
			markie(c, params, KICK);
			if (c->getNick() == params[2])
				ad = i;
		}
		i++;
	}
	if (us > -1)
		ch->getUsers().erase(ch->getUsers().begin() + us);
	if (ad > -1)
		ch->getAdmins().erase(ch->getAdmins().begin() + ad);
	std::cout << "KICK REMOVING AND MARKING" << std::endl;

}

void	Command::topic(std::vector<std::string> params)
{
	if (params.size() <= 1)
		return msgError(ERR_NEEDMOREPARAMS, params);
	Channel *ch = this->getChannelByName(params[1]);
	if (!ch)
		return msgError(ERR_NOSUCHCHANNEL, params);
	if (ch->getTopicRestriction())
	{
		if (!ch->isAdmin(launcher->getNick()))
		{
			if (ch->isUser(launcher->getNick()))
				return msgError(ERR_CHANOPRIVSNEEDED, params);
			else
			{
				params.push_back(ch->getId());
				return msgError(ERR_NOTONCHANNEL, params);
			}
		}
	}
	if (!ch->isAdmin(launcher->getNick()) && !ch->isUser(launcher->getNick()))
	{
		params.push_back(ch->getId());
		return msgError(ERR_NOTONCHANNEL, params);
	}
	std::string topicmsg("");
	for (size_t i = 2; i < params.size(); i++)
	{
		topicmsg += " ";
		topicmsg += params[i];
	}
	std::vector<std::string> chans;

	if (params.size() == 2)
	{
		if (ch->getTopic().empty())
		{
			markEverything(NOTOPIC, params);
			launcher->setPollOut(1);
			return ;
		}
	}
	if (topicmsg != "")
		ch->setTopic(topicmsg);
	chans.push_back(params[1]);
	markChannelPollOut(channels, chans, params, TOPIC_OK);
	markEverything(TOPIC_OK, params);
	launcher->setPollOut(1);
}


void	Command::part(std::vector<std::string> params)
{
	int	ad;
	int	us;

	if (params.size() < 2)
		return msgError(ERR_NEEDMOREPARAMS, params);
	std::vector<std::string> chanToLeave = split(params[1], ',');
	for (std::vector<std::string>::iterator it = chanToLeave.begin(); it != chanToLeave.end(); it++)
	{
		Channel *ch = this->getChannelByName(*it);
		if (!ch)
		{
			msgError(ERR_NOSUCHCHANNEL, params);
			continue ;
		}
		ch->printAdminUsers();
		std::vector<std::string*> users = ch->getUsers();
		std::vector<std::string*> admins = ch->getAdmins();
		if  (!ch->isUser(launcher->getNick()) && !ch->isAdmin(launcher->getNick()))
		{
			params.push_back(ch->getId());
			msgError(ERR_NOTONCHANNEL, params);
			continue ;
		}
		for (size_t i = 0; i < users.size(); i++)
		{
			Client *c = findClientByNick(*users.at(i));
			if (c && c != &(*(clients->end())))
			{
				if (c->getNick() == launcher->getNick())
					us = i;
				markie(c, params, PART);
			}
		}
		for (size_t i = 0; i < admins.size(); i++)
		{
			Client *c = findClientByNick(*admins.at(i));
			if (c && c != &(*(clients->end())))
			{
				if (c->getNick() == launcher->getNick())
					ad = i;
				markie(c, params, PART);
			}
		}
		if (ch->isAdmin(launcher->getNick()))
		{
			std::vector<std::string*>::iterator it = ch->getAdmins().begin() + ad;
			ch->getAdmins().erase(it);
		}
		if (ch->isUser(launcher->getNick()))
		{
			std::vector<std::string*>::iterator it = ch->getUsers().begin() + us;
			ch->getUsers().erase(it);
		}
		ch->printAdminUsers();
	}
	markEverything(PART, params);
	launcher->setPollOut(1);
}

void	Command::invite(std::vector<std::string> params)
{
	if (params.size() != 3)
	{
		return msgError(ERR_NEEDMOREPARAMS, params);
	}
	Channel *ch = this->getChannelByName(params[2]);
	if (!ch)
	{
		return msgError(ERR_NOSUCHCHANNEL, params);
	}
	if (!ch->isAdmin(launcher->getNick()))
	{
		if (ch->isUser(launcher->getNick()))
			return msgError(ERR_CHANOPRIVSNEEDED, params);
		else
			return msgError(ERR_USERNOTINCHANNEL, params);
	}
	if  (ch->isUser(params[1]) || ch->isAdmin(params[1]))
		return msgError(ERR_USERONCHANNEL, params);
	std::vector<std::string> target;

	target.push_back(params[1]);
	Client *cl = findClientByNick(params[1]);
	ch->setGuest(&cl->getNick());
	markClientsPollOut(clients, target, params, INVITE);
}

void	Command::kick(std::vector<std::string>& params)
{
	size_t	i;
	size_t	pos;

	i = 0;
	std::cout << "WEEEEEEEEBA" << std::endl;
	if (params.size() < 3)
		return msgError(ERR_NEEDMOREPARAMS, params);
	Channel *ch = this->getChannelByName(params[1]);
	if (!ch)
	{
		return msgError(ERR_NOSUCHCHANNEL, params);
	}
	if (!ch->isAdmin(launcher->getNick()))
	{
		if (ch->isUser(launcher->getNick()))
			return msgError(ERR_CHANOPRIVSNEEDED, params);
		params.push_back(ch->getId());
		return msgError(ERR_NOTONCHANNEL, params);
	}
	if (!ch->isAdmin(params[2]) && !ch->isUser(params[2]))
	{
		return msgError(ERR_USERNOTINCHANNEL, params);
	}
	std::vector<std::string> chans = split(params[1], ',');
	std::vector<std::string> users = split(params[2], ',');
	std::vector<std::string> parameters;

	parameters.push_back("KICK");
	chans.size() >= users.size() ? i = chans.size() : i = users.size();
	pos = 0;
	while (pos < i)
	{
		if (pos >= chans.size() || pos >= users.size())
		{
			markEverything(ERR_NEEDMOREPARAMS, params);
			launcher->setPollOut(1);
			return ;
		}
		parameters.push_back(chans.at(pos));
		parameters.push_back(users.at(pos));
		if (params.size() >= 3)
		{
			for (size_t i = 0; i < params.size(); i++)
				parameters.push_back(params.at(i));
		}
		kickProcessOne(parameters);
		parameters.clear();
		parameters.push_back("KICK");
		pos++;
	}
}


		

void	Command::kickProcessOne(const std::vector<std::string>& params)
{
	Channel *ch = this->getChannelByName(params[1]);
	markChannel(ch, params);
}

bool	Command::isModeOption(char c)
{
	if (c == 'i' || c == 'l' || c == 'o' || c == 'k' || c == 't')
		return 1;
	return 0;
}

bool	Command::oFlagChecker(std::string option)
{
	for (size_t i = 0; i < option.size(); i++)
	{
		if (option[i] != 'o' && option[i] != '+' && option[i] != '-')
			return 0;
	}
	return 1;
}

void	Command::extractFlag(size_t pos)
{
	if (pos >= optOff.size())
		return ;
	std::string::iterator it = optOff.begin() + pos;

	this->optOff.erase(it);
}




int Command::plusMode(Channel *ch, std::vector<std::string> params, char opt, char sign, size_t pos)
{
	size_t	paramsBegin = 2;

	while (paramsBegin < params.size() && (params[paramsBegin].find('+') != std::string::npos || params[paramsBegin].find('-') != std::string::npos))
		paramsBegin++;
			
	for (size_t i = 0; i < params.size() ; i++)
	{
		std::cout << "EE : " << params[i] << std::endl;
	}
	if (opt == 'i')
	{
		sign == '+' ? ch->setInviteMode(1) : ch->setInviteMode(0);
		optOff.push_back(sign);
		optOff.push_back(opt);
	}
	else if (opt == 'l')
	{
		if (sign == '+')
		{
			if (pos + paramsBegin >= params.size())
				return -1;
			std::string arg = params.at(pos + paramsBegin);
			for (size_t i = 0; i < arg.size(); i++)
			{
				if (!std::isdigit(arg[i]))
					return -2;
			}
			ch->setMaxUsers(std::atoi(arg.c_str()));
			argOff.push_back(arg);
		}
		else
		{
			if (pos + paramsBegin < params.size())
				return -1;
			ch->setMaxUsers(-1);
		}
		optOff.push_back(sign);
		optOff.push_back(opt);
	}
	else if (opt == 't')
	{
		sign == '+' ? ch->setTopicRestriction(1) : ch->setTopicRestriction(0);
		optOff.push_back(sign);
		optOff.push_back(opt);
	}
	else if (opt == 'k')
	{
		if (sign == '+')
		{
			if (pos + paramsBegin >= params.size())
				return -1;
			ch->setPassword(params.at(pos + paramsBegin));
			argOff.push_back(params.at(pos + paramsBegin));
		}
		else
		{
			if (pos + paramsBegin < params.size())
				return -1;
			std::string psswd;
			ch->setPassword(psswd);
		}
		optOff.push_back(sign);
		optOff.push_back(opt);
	}
	else if (opt == 'o')
	{
		std::vector<int>	position;
		std::vector<int>	positionAd;
		if (pos + paramsBegin >= params.size())
			return -1;
		std::string arg = params.at(pos + paramsBegin);
		std::cout << "'O' FLAG " << std::endl;
		ch->printAdminUsers();
		if (!ch->isAdmin(arg) && !ch->isUser(arg))
			return -7;
		for (size_t y = 0; y < ch->getUsers().size(); y++)
		{
			if (*(ch->getUsers().at(y)) == arg)
			{
				position.push_back(y);
			}
		}
		for (size_t y = 0; y < ch->getAdmins().size(); y++)
		{
			if (*(ch->getAdmins().at(y)) == arg)
			{
				positionAd.push_back(y);
			}
		}
		for (std::vector<int>::iterator it = position.begin(); it != position.end(); it++)
		{
			if (sign == '+' && !ch->isAdmin(arg))
			{
				std::vector<std::string*>::iterator nickIT = ch->getUsers().begin() + *it;
				ch->getAdmins().push_back(*nickIT);
				ch->getUsers().erase(nickIT);
			}
			else if (sign == '-' && !ch->isUser(arg))
			{
				std::vector<std::string*>::iterator nickit = ch->getAdmins().begin() + *it;
				ch->getUsers().push_back(*nickit);
				ch->getAdmins().erase(nickit);
			}
		}
		for (std::vector<int>::iterator it = positionAd.begin(); it != positionAd.end(); it++)
		{
			if (sign == '-')
			{
				std::vector<std::string*>::iterator nicke = ch->getAdmins().begin() + *it;
				std::cout << "nick :::::=> " << **nicke << std::endl;
				ch->getUsers().push_back(*nicke);
				ch->getAdmins().erase(nicke);
			}
		}
		ch->printAdminUsers();
		optOff.push_back(sign);
		optOff.push_back(opt);
		argOff.push_back(arg);
	}
	return 1;
}


std::string	Command::options(std::vector<std::string> params)
{
	size_t	i;
	std::string options(params[2]);

	i = 3;
	while (i < params.size())
	{
		while (i < params.size() && (params[i].find('+') != std::string::npos || params[i].find('-') != std::string::npos))
			options += params[i++];
		break;
		i++;
	}
	return options;
}

std::vector<std::string> Command::rmErrorArgs(std::vector<std::string> params, std::vector<int> rm, size_t p)
{
	int	u;
	std::vector<std::string> neoOpt;

	u = 0;
	for (std::vector<int>::iterator it = rm.begin(); it != rm.end(); it++)
	{
		for (size_t i = p; i < params.size(); i++)
		{
			if (u != *it)
				neoOpt.push_back(params.at(i));
			u++;
					
		}
	}
	if (neoOpt.empty())
		return params;
	return neoOpt;
}




std::string Command::rmErrorFlags(std::string opt, std::vector<int> rm)
{
	int	u;
	std::string neoOpt;

	u = 0;
	for (size_t i = 0; i < opt.size(); i++)
	{
		for (std::vector<int>::iterator it = rm.begin(); it != rm.end(); it++)
		{
			if (u != *it)
			{
				if (opt[i] != '+' || opt[i] != '-')
					u++;
				neoOpt.push_back(opt[i]);
			}
			else
				u++;
		}

	}
	if (neoOpt.empty())
		return opt;
	return neoOpt;
}



void	Command::modeInstructions(Channel *ch, std::vector<std::string> params)
{
	std::string	opt = options(params);
	std::vector<int>	we;
	size_t		i;
	char		sign;
	size_t		pos;
	int		status;
	
	i = 0;
	pos = 0;
	while (i < opt.size())
	{

		if (opt.at(i) == '+' || opt.at(i) == '-')
		{
			sign = opt.at(i);
			i++;
		}
		if (i >= opt.size() || !isModeOption(opt.at(i)))
		{
			markEverything(ERR_UNKNOWNMODE, params);
			launcher->setPollOut(1);
			return ;
		}
		status = plusMode(ch, params, opt.at(i++), sign, pos);
		if (status == -1)
		{
			markEverything(ERR_NEEDMOREPARAMS, params);
			launcher->setPollOut(1);
			//return ;
		}
		else if (status == -2 || status == -3)
		{
			markEverything(ERR_UNKNOWNMODE, params);
			launcher->setPollOut(1);
			//return ;
		}
		else if (status == -7)
		{
			markEverything(ERR_USERNOTINCHANNEL, params);
			launcher->setPollOut(1);
			//return ;
		}
		pos++;
	}
	std::vector<std::string> target;

	target.push_back(ch->getId());
	
	std::vector<std::string> parameters;

	parameters.push_back(params[0]);
	parameters.push_back(params[1]);
	parameters.push_back(this->optOff);
	for (size_t i = 0; i < this->argOff.size(); i++)
		parameters.push_back(argOff.at(i));
	for (size_t i = 0; i < parameters.size(); i++)
		std::cout << parameters[i] << std::endl;
	if (optOff.size() > 0)
	{
		markChannelPollOut(channels, target, parameters, MODE_OK);
		markEverything(MODE_OK, parameters);
		launcher->setPollOut(1);
	}
}
	
void	Command::modeOptions(Channel *ch, std::vector<std::string> params)
{
	std::stringstream ss;
	if (params.size() == 2)
	{
		if (ch->getPassword() != "")
			params.push_back("+k " + ch->getPassword());
		if (ch->getMaxUsers() > 0)
		{
    			ss << ch->getMaxUsers();
    			std::string maxusers = ss.str();
			params.push_back("+l " + maxusers);
		}
		if (ch->getInviteMode())
			params.push_back("+i");
		if (ch->getTopicRestriction())
			params.push_back("+t");
		markEverything(MODE_WITH_CHANNEL, params);
		launcher->setPollOut(1);
		return ;
	}
	if (!ch->isAdmin(launcher->getNick()))
	{
		return msgError(ERR_CHANOPRIVSNEEDED, params);
	}
	modeInstructions(ch, params);
}



void	Command::mode(std::vector<std::string> params)
{
	if (params.size() < 2)
	{
		return msgError(ERR_NEEDMOREPARAMS, params);
	}
	Channel *ch = this->getChannelByName(params[1]);
	if (!ch)
	{
		return msgError(ERR_NOSUCHCHANNEL, params);
	}
	ch->printAdminUsers();
	if (!ch->isAdmin(launcher->getNick()) && !ch->isUser(launcher->getNick()))
	{
		return msgError(ERR_USERNOTINCHANNEL, params);
	}
	modeOptions(ch, params);
}









void	Command::join(const std::vector<std::string>& params, str &userId, std::string chaName, size_t pos)
{
	str password;
	if (params.size() == 3)
	{
		std::vector<std::string> pass = split (params[2], ',');
		if (pos >= pass.size())
			password = "";
		else
			password = pass[pos];
	}
	Channel *channel = getChannelByName(chaName);
	if (!channel)
	{
		if ((chaName).at(0) != '&' && (chaName).at(0) != '#')
			throw std::logic_error("ERR_BADCHANMASK");
		Channel ch = Channel(chaName);
		ch.joinClient(userId, password, true);
		channels->push_back(ch);

	}
	else
	{
		std::cout << "PRESHOIN ------> " << std::endl;
		channel->printAdminUsers();
		channel->joinClient(userId, password, false);
		std::cout << "POSTSHOIN ------> " << std::endl;
		channel->printAdminUsers();
	}
}

vectorCh::iterator	Command::getChannel(const str& id)
{
	vectorCh::iterator channel = channels->begin();
	while (channel != channels->end() && channel->getId() != id)
		++channel;
	return channel;
}


int Command::stringToEnum(const str& str) {
    static std::map<std::string, int> stringToEnumMap;

    if (stringToEnumMap.empty()) {
        stringToEnumMap["CMD_ERROR"] = CMD_ERROR;
        stringToEnumMap["CMD_NOTFOUND"] = CMD_NOTFOUND;
        stringToEnumMap["NICK_OK"] = NICK_OK;
        stringToEnumMap["NICK_REPEATED"] = NICK_REPEATED;
        stringToEnumMap["WELCOME"] = WELCOME;
        stringToEnumMap["QUIT"] = QUIT;
        stringToEnumMap["PART"] = PART;
        stringToEnumMap["CAP_LS"] = CAP_LS;
        stringToEnumMap["CAP_END"] = CAP_END;
        stringToEnumMap["PRIVMSG"] = PRIVMSG;
        stringToEnumMap["BAD_PSSWD"] = BAD_PSSWD;
        stringToEnumMap["USER"] = USER;
        stringToEnumMap["OK_PSSWD"] = OK_PSSWD;
        stringToEnumMap["JOIN_OK"] = JOIN_OK;
        stringToEnumMap["MODE_OK"] = MODE_OK;
        stringToEnumMap["KICK"] = KICK;
        stringToEnumMap["INVITE"] = INVITE;
        stringToEnumMap["MODE"] = MODE;
        stringToEnumMap["TOPIC_OK"] = TOPIC_OK;
        stringToEnumMap["NOTOPIC"] = NOTOPIC;
        stringToEnumMap["MODE_WITH_CHANNEL"] = MODE_WITH_CHANNEL;
        stringToEnumMap["ERR_NOTONCHANNEL"] = ERR_NOTONCHANNEL;
        stringToEnumMap["ERR_USERONCHANNEL"] = ERR_USERONCHANNEL;
        stringToEnumMap["ERR_BADCHANMASK"] = ERR_BADCHANMASK;
        stringToEnumMap["ERR_NEEDMOREPARAMS"] = ERR_NEEDMOREPARAMS;
        stringToEnumMap["ERR_ALREADYREGISTRED"] = ERR_ALREADYREGISTRED;
        stringToEnumMap["ERR_UNKNOWNMODE"] = ERR_UNKNOWNMODE;
        stringToEnumMap["ERR_NOSUCHCHANNEL"] = ERR_NOSUCHCHANNEL;
        stringToEnumMap["ERR_CHANOPRIVSNEEDED"] = ERR_CHANOPRIVSNEEDED;
        stringToEnumMap["ERR_NOSUCHNICK"] = ERR_NOSUCHNICK;
        stringToEnumMap["ERR_USERNOTINCHANNEL"] = ERR_USERNOTINCHANNEL;
        stringToEnumMap["ERR_INVITEONLYCHAN"] = ERR_INVITEONLYCHAN;
        stringToEnumMap["ERR_CHANNELISFULL"] = ERR_CHANNELISFULL;
        stringToEnumMap["ERR_NOSUCHCHANNEL"] = ERR_NOSUCHCHANNEL;
        stringToEnumMap["ERR_BADCHANNELKEY"] = ERR_BADCHANNELKEY;
        stringToEnumMap["RPL_TOPIC"] = RPL_TOPIC;
        stringToEnumMap["RPL_NAMREPLY"] = RPL_NAMREPLY;
        stringToEnumMap["ERR_BADCHANNELID"] = ERR_BADCHANNELID;
        stringToEnumMap["RPL_WHOREPLY"] = RPL_WHOREPLY;
        stringToEnumMap["RPL_ENDOFWHO"] = RPL_ENDOFWHO;
    }

    std::map<std::string, int>::const_iterator it = stringToEnumMap.find(str);
    if (it != stringToEnumMap.end()) {
        return it->second;
    } else {
        return CMD_NOTFOUND;
    }
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::string part;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (*it == delimiter) {
            if (!part.empty()) {
                parts.push_back(part);
                part.clear();
            }
        } else {
            part += *it;
        }
    }
    if (!part.empty()) {
        parts.push_back(part);
    }
    return parts;
}

