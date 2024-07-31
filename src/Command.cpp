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
		case(MODE_OK):
			std::cout << "WEEEE" << std::endl;
			response = ":@" + launcher->getNick() + " MODE";
			for (size_t i = 1; i < params.size(); i++)
			{
				response += " ";
				response += params.at(i);
			}
			break;
		case(KICK):
			//std::cout << "EEEEEEGE PASA AQUII CON EL KIICK" << std::endl;
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
			if (params.size() > 2)
			{
				for (std::vector<std::string>::iterator it = params.begin() + 2; it != params.end(); it++)
				{
					response += " ";
					response += *it;
				}
			}
			break;
		case(MODE_WITH_CHANNEL):
			response += " 324 " + launcher->getNick() + " " + params.at(1) + " :";
			for (size_t i = 2; i < params.size(); i++)
				response += params.at(i);
			break;
		case(ERR_NOTONCHANNEL):
			response += " 442 " + launcher->getNick() + " " + params[1] + " :You are not in this channel";
			break;
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
				response += " 381 " + launcher->getNick() + " :Nick " + launcher->getNick() + " magnificent set!";
			}
			else
			{
				response = ":" + launcher->getNick() + " NICK " + params[1];
				//response = std::string(":") + std::string(" NICK ") + params[1];
				launcher->setNick(params[1]);
			}
			break;
		case(NICK_REPEATED):
			response += " 433 * " + params[1] + " :Nickname is already in use.";
			break;
		case(ERR_ERRONEUSNICKNAME):
			response += " 432 " + launcher->getUser() + " " + params[1];
			break;
		case(ERR_BADCHANNELID):
			response +=  " 476 " + launcher->getNick() +  " " + params[1] + " :Bad Channel name";
			break;
		case(QUIT):
			if (params.size() <= 1)
				response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " QUIT";
				//response += " QUIT: No leaving message";
			else
			{
				response = ":" + launcher->getNick() + "!" + launcher->getUser() + "@" + hostName + " QUIT ";
				for (size_t i = 1; i < params.size(); i++)
				{
					response += " ";
					response += params[i];
				}
				/*std::string quitMsg(&params[1].c_str()[1]);
				response += " QUIT: ";
				response += quitMsg;
				*/
			}
			break;
		case(ERR_NEEDMOREPARAMS):
			response += " 461 ";
			response += params[0];
			response += " :Not enough or too much params for ";
			response += params[0];
			break;
		case(ERR_NOSUCHCHANNEL):
			response += " 403 " + launcher->getNick() + " :No such a channel";
			break;
		case(ERR_CHANNELISFULL):
			response += " 471 " + launcher->getNick() + " " + params[1] + " :This channel is full";
			break;
		case(ERR_CHANOPRIVSNEEDED):
			response += " 482 " + launcher->getNick() + " :You are not a chop";
			break;
		case(ERR_USERNOTINCHANNEL):
			response += " 441 " + launcher->getNick() + " :User not in channel";
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
			response = ":";
			if (params[1].at(0) == '#' || params[1].at(0) == '&')
			{	
				Channel *ch = getChannelByName(params[1]);
				if (ch->isAdmin(launcher->getNick()))
					response += "@";
			}
			response += launcher->getNick();
			response += " PRIVMSG " + params[1] +  " ";
			for (size_t  i = 2; i < params.size(); i++)
			{
				response += params[i];
				response += " ";
			}
			break;
		case(WHO_BEGIN):
			response += " 352 " + launcher->getNick() + " " + params[1] + " " +  launcher->getUser() + " " +  hostName + " " +  launcher->getNick() + " H :0 " + launcher->getUser();// <flags> :<hopcount> <real name>
															break;
														case(WHO_END):
			response += " 315 " + launcher->getNick() + " " + params[1] + " :End of list";

			break;
		case(JOIN_OK):
			response = ":" + launcher->getNick() + " JOIN " + params[0];
			break;
		case(ERR_BADCHANNELKEY):
			response = params[1] + " :Cannot join channel (+k)";
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
			/*if (params[0] == "NICK")
				std::cout << *y << std::endl;
				*/
		}
		markAction(params);
		if (launcher->getOff() == 1)
			return ;
		params.clear();
	}
}

void	Command::markAction(std::vector<std::string> params)
{
	std::string cmd = params[0];
	/*std::cout << "Params: {" << std::endl;
	for (std::size_t i = 0; i < params.size(); i++)
	{
		std::cout << " " << params[i];
	}
	std::cout << " }" << std::endl;
	//std::cout << cmd << " : " << params.size()<< std::endl;;
	//std::cout << "iusa" << std::endl;
	//std::cout << cmd << std::endl;
	*/
	std::cout << " LO QUE EVALUAMOS " << std::endl;
	std::cout << cmd << std::endl;
	for (std::vector<std::string>::iterator it = params.begin() ; it != params.end(); it++)
		if (*it == "\r\n")
			params.erase(it);
	if (cmd == "PASS")
		pass(params);
	else if (cmd == "NICK")
	{
		//std::cout << "HOLIIIIII!!!" << std::endl;
		//for (size_t i = 0; i <  params.size(); i++)
		//	std::cout << params[i] << std::endl;
		nick(params);
	}
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
	else if (cmd == "PRIVMSG")
	{
		//std::cout << "comienza" << std::endl;
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		privmsg(params);
	}
	else if (cmd == "KICK")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		kick(params);
	}
	else if (cmd == "MODE")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		std::cout << "UUUUMPTAGUMPDTA " << params.at(params.size() - 1) << std::endl;
		write(1, &params.at(params.size() - 1)[0], 1);
		if (params.at(params.size() - 1).find("\r\n") != std::string::npos)
			std::cout << "WWWWWWWWWWAAAAAAAAAAAAA" << std::endl;
		mode(params);
	}
	else if (cmd == "JOIN")
	{
		//std::cout << "el comando es join" << std::endl;
		//std::cout << "El nick de la clase cliente que ejecuta el join es: " << launcher->getUser() << std::endl;
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		int responseType;
	
		try
		{
			join(params, &launcher->getNick());
			responseType = JOIN_OK;
		}
		catch(std::exception& e)
		{
			responseType = stringToEnum(e.what());
			std::cout << e.what() << std::endl;
		}
		markEverything(responseType, params);
		launcher->setPollOut(1);
	}
	/*else if (cmd == "WHO")
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		who(params);
	}*/
	else if (cmd == "")
		return ;
	else
	{
		if (!launcher->getRegister())
		{
			notRegistered(params);
			return ;
		}
		markEverything(CMD_NOTFOUND, params);
		launcher->setPollOut(1);
		//launcher->setCommand(CMD_NOTFOUND);
	}
/*
*/
}

void	Command::notRegistered(std::vector<std::string> params)
{
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
	if (params[1] == "LS")
		launcher->setCommand(CAP_LS);
	else if (params[1] == "END")
		launcher->setCommand(CAP_END);
	
	markEverything(launcher->getCommand(), params);
	/*response = responseGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
	*/
	//launcher->setPollOut(1);
	//send(launcher->getSocket(), response.c_str(), response.size(), 0);
}


std::vector<std::string> Command::split(std::string na, const char *c)
{
	std::vector<std::string> tokens;
	std::string		 token;
	size_t			plus;
	std::string		n = na;
	size_t pos;

	plus = strlen(c);
	if (n.find(c) == std::string::npos)
	{
		tokens.push_back(na);
	}
	while ((pos = n.find(c)) != std::string::npos)
	{
		token = n.substr(0, pos);
		tokens.push_back(token);
		n.erase(0, pos + plus);
		if ((pos = n.find(c)) == std::string::npos)
		{
			token = n.substr(0, pos);
			tokens.push_back(token);
			break ;
		}
	}
	return tokens;
}

std::vector<std::string> Command::split(std::string na, const char c)
{
	std::string n(na);
	size_t pos;
	std::vector<std::string> tokens;
	std::string		 token;

	if (n.find(c) == std::string::npos)
		tokens.push_back(na);
	pos = n.find(c);
	while (n.find(c) && pos != std::string::npos)
	{
		token = n.substr(0, pos);
		tokens.push_back(token);
		while (n.at(pos) == c)
		{
			pos++;
			if (pos == n.size())
				break ;
		}
		n.erase(0, pos);
		pos = n.find(c);
		if (pos == std::string::npos)
		{
			token = n.substr(0, pos);
			tokens.push_back(token);
			break ;
		}
	}
	return tokens;
}


//aver que hacer con mas de 512 bytes
int Command::parseMsg()
{
	std::vector<std::string> tokens;
	std::vector<std::string> parameters;

	if (msg.find("\r\n") == std::string::npos || msg.size() >= 512)
	{
		return (2);
	}
	tokens = split(msg, "\r\n");
	for (size_t i = 0; i < tokens.size(); i++)
	{
	
		std::cout << "QOHIJDIHNISCNBIBCIKBCD" << std::endl;
		parameters = split(tokens[i], ' ');
		std::cout << "QOHIJDIHNISCNBIBCIKBCD" << std::endl;
		cmd[i] = parameters;
		parameters.clear();
	}
	return (0);
}

void	Command::markEverything(int cmd, std::vector<std::string> params)
{
	std::string response;

	//launcher->setStatus(ERROR);
	launcher->setCommand(cmd);
	if (cmd == JOIN_OK)
	{
		std::vector<std::string> channels_name;
		std::vector<std::string> parameters;

		channels_name = split(params[1], ",");
		for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); it++)
		{
			std::cout << "PASA TANTAS VECES POR LOS NOMBRES DEL LOS CANALES" << std::endl;
			parameters.push_back(*it);
			response = responseGenerator(launcher->getCommand(), parameters);
			std::string isempty = response.substr(1, response.size());
			if (isempty != hostName + "\r\n")
			{
				launcher->setResponse(response);
				launcher->setNCmd(launcher->getCommand());
			}
			parameters.clear();
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
	{
		//std::cout << "aaa" << std::endl;
		launcher->setPollOut(1);
	}
	else
		welcome(params);
	return 0;
}

int	Command::user(std::vector<std::string> params)
{
	std::string response;
	if (params.size() < 5)
	{
		markEverything(ERR_NEEDMOREPARAMS, params);
		launcher->setPollOut(1);
		return 3;
	}
	launcher->setUser(params[1]);
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
		if ((nick[pos] >= 'A' && nick[pos] <= '^') || (nick[pos] >= '`' && nick[pos] <= '{') || nick[pos] == '}')
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
		return 3;
	}
	if (params.size() != 2)
	{
		markEverything(ERR_NEEDMOREPARAMS, params);
		launcher->setPollOut(1);
		return 3;
	}
	if (params[1].size() > 9 || parseNick(params[1]) != 1)
	{
		markEverything(ERR_ERRONEUSNICKNAME, params);
		launcher->setPollOut(1);
		return 3;
	}
	for (std::vector<Client >::iterator it = clients->begin(); it != clients->end(); it++)
	{
		if ((*it).getNick() == params[1])
		{
			markEverything(NICK_REPEATED, params);
			launcher->setPollOut(1);
			return 2;
		}
	}
	//if (launcher->getRegister() == 0)
	//	launcher->setNick(params[1]);
	markEverything(NICK_OK, params);
	//if (launcher->getRegister() == 1)
	//	launcher->setNick(params[1]);
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
		std::cout << "HOLAAAAAAAAAAA" << std::endl;
		if (cl->getNick() == nick)
			return &(*cl);
	}
	return (&(*clients->end()));
}

void	Command::quit(std::vector<std::string> params)
{
	launcher->setOff(1);
	std::vector<std::string> nicks;
	std::cout << "Channels size when quitting: "<< channels->size() <<std::endl;

	markEverything(QUIT, params);
	launcher->setPollOut(1);

	/*for (vectorCh::iterator ch = channels->begin(); ch != channels->end(); ch++)
	{
		while (i < ch->getUsers().size())
		{
			sendQuit.push_back(ch->getUsers()[i++]);
		}
		//ch->deleteClient(launcher->getNick());
	}
	for (vectorCh::iterator ch = channels->begin(); ch != channels->end(); ch++)
	{
		while (i < ch->getUsers().size())
		{
			sendQuit.push_back(ch->getAdmins()[i++]);
		}
	}
	std::cout << "Number of clients to quit: "<< sendQuit.size() <<std::endl;
	for (std::vector<std::string>::iterator nick = sendQuit.begin(); nick != sendQuit.end(); nick++)
	{
		Client 		*sendQuitMsg;
		std::string	response;

		sendQuitMsg = findClientByNick(*nick);
		if (sendQuitMsg == &(*clients->end())) //|| sendQuitMsg->getNick() == launcher->getNick()) 
		{
			std::cout << "BYYYE MAAATE "<< std::endl;
			continue ;
		}
		std::cout << "HELLO MAAATE " << sendQuitMsg->getNick() << std::endl;
		popAllCoincidences(&sendQuit, *nick);
		sendQuitMsg->setNCmd(BLANK);
		sendQuitMsg->setCommand(QUIT);
		response = responseGenerator(sendQuitMsg->getCommand(), params);
		sendQuitMsg->setResponse(response);
		sendQuitMsg->setPollOut(1);
	}*/
	//launcher->setOff(1);
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

void	Command::markClientsPollOut(std::vector<Client> *clients, std::vector<std::string> dest, std::vector<std::string> params)
{
	for (size_t y = 0; y < dest.size(); y++)
	{
		for (size_t i = 0; i < clients->size(); i++)
		{
			if (dest[y] == clients->at(i).getNick())
			{
				markie(&clients->at(i), params, PRIVMSG);
			}
		}
	}
}

/*
std::vector<std::string>	Command::getChannelNicks(Channel ch)
{
	std::vector<std::string> nicks;

	for  (size_t j = 0; j < ch.getUsers().size(); j++)
	{
		nicks.push_back(ch.getUsers().at(j));
	}
	for  (size_t j = 0; j < ch.getAdmins().size(); j++)
	{
		nicks.push_back(ch.getAdmins().at(j));
	}
	return nicks;
}
*/


void	Command::markChannelPollOut(std::vector<Channel> *ch, std::vector<std::string> dest, std::vector<std::string> params, int type)
{
	std::cout << "es aqui de calle" << std::endl;
	for (size_t y = 0; y < dest.size(); y++)
	{
		for (size_t i = 0; i < ch->size(); i++)
		{
			if (dest[y] == ch->at(i).getId())
			{
				for  (size_t j = 0; j < ch->at(i).getUsers().size(); j++)
				{
					if (*(ch->at(i).getUsers().at(j)) == launcher->getNick())
						continue ;
					Client *cl = findClientByNick(*(ch->at(i).getUsers().at(j)));
					if (cl)
						markie(cl, params, type);
				}
				for  (size_t j = 0; j < ch->at(i).getAdmins().size(); j++)
				{
					if (*(ch->at(i).getAdmins().at(j)) == launcher->getNick())
						continue ;
					Client *cl = findClientByNick(*(ch->at(i).getAdmins().at(j)));
					if (cl)
						markie(cl, params, type);
				}
			}
		}
	}
	std::cout << "pero de calle" << std::endl;
}

void	Command::privmsg(std::vector<std::string> params)
{
	std::vector<std::string>	dest;
	std::string			response;

	if (params.size() < 3)
	{
		//caso de error
		launcher->setCommand(ERR_NEEDMOREPARAMS);
		response = responseGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
		launcher->setPollOut(1);
		return ;
	}
	//caso respuesta valida
	dest = split(params[1], ",");
	markClientsPollOut(clients, dest, params);
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
	std::cout << "--------------------" << std::endl;
	ch->printAdminUsers();
	
	for (std::vector<std::string*>::iterator u = ch->getUsers().begin() ; u != ch->getUsers().end(); u++)
	{
		Client *c = findClientByNick(**u);
		if (c != &(*(clients->end())))
		{
			markie(c, params, KICK);
		}
	}
	for (std::vector<std::string*>::iterator u = ch->getAdmins().begin(); u != ch->getAdmins().end(); u++)
	{
		Client *c = findClientByNick(**u);
		if (c != &(*(clients->end())))
			markie(c, params, KICK);
	}
	//std::vector<std::string *>::iterator posRm = ch->getUsers().begin();
	for (std::vector<std::string *>::iterator rm = ch->getUsers().begin(); rm != ch->getUsers().end(); rm++)
	{
		if (**rm == params[1])
			ch->getUsers().erase(rm);
	}
	for (std::vector<std::string *>::iterator rm = ch->getAdmins().begin(); rm != ch->getAdmins().end(); rm++)
	{
		if (**rm == params[1])
			ch->getAdmins().erase(rm);
	}
	ch->printAdminUsers();
	std::cout << "--------------------" << std::endl;
}
		
void	Command::part(std::vector<std::string> params)
{
	if (params.size() < 2)
	{
		return msgError(ERR_NEEDMOREPARAMS, params);
	}
	std::vector<std::string> chanToLeave = split(params[1], ",");
	for (std::vector<std::string>::iterator it = chanToLeave.begin(); it != chanToLeave.end(); it++)
	{
		Channel *ch = this->getChannelByName(*it);
		ch->printAdminUsers();
		if  (!ch->isUser(launcher->getNick()) && !ch->isAdmin(launcher->getNick()))
			return msgError(ERR_NOTONCHANNEL, params);
		for (std::vector<std::string*>::iterator u = ch->getUsers().begin() ; u != ch->getUsers().end(); u++)
		{
			std::cout << "ahi va: " << *u << " ---- " << **u << std::endl;
			Client *c = findClientByNick(**u);
				std::cout << "weeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
			if (c && c != &(*(clients->end())))
			{
				std::cout << "weeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
				markie(c, params, PART);
			}
		}
		for (std::vector<std::string*>::iterator u = ch->getAdmins().begin() ; u != ch->getAdmins().end(); u++)
		{
			Client *c = findClientByNick(**u);
			if (c && c != &(*(clients->end())))
			{
				markie(c, params, PART);
			}
		}
		for (std::vector<std::string *>::iterator rm = ch->getUsers().begin(); rm != ch->getUsers().end(); rm++)
		{
			if (**rm == launcher->getNick())
				ch->getUsers().erase(rm);
		}
		for (std::vector<std::string *>::iterator rm = ch->getAdmins().begin(); rm != ch->getAdmins().end(); rm++)
		{
			if (**rm == launcher->getNick())
				ch->getAdmins().erase(rm);
		}
		ch->printAdminUsers();
	}
}

void	Command::kick(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return msgError(ERR_NEEDMOREPARAMS, params);
	}
	Channel *ch = this->getChannelByName(params[1]);
	if (!ch)
	{
		return msgError(ERR_NOSUCHCHANNEL, params);
	}
	if (!ch->isAdmin(launcher->getNick()))
	{
		if (ch->isUser(launcher->getNick()))
			return msgError(ERR_CHANOPRIVSNEEDED, params);
		return msgError(ERR_USERNOTINCHANNEL, params);
	}
	if (!ch->isAdmin(params[2]) && !ch->isUser(params[2]))
	{
		return msgError(ERR_USERNOTINCHANNEL, params);
	}
	markChannel(ch, params);
	//markEverything(KICK, params);
	//launcher->setPollOut(1);
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


int Command::plusMode(Channel *ch, std::vector<std::string> params, std::string options)
{
	size_t	i = 0;
	size_t	pos = 0;
	char sign;

	std::cout << params.size() << std::endl;
	std::cout << options.size() << std::endl;
	for (size_t i = 0; i < params.size() ; i++)
	{
		std::cout << "EE : " << params[i] << std::endl;
	}
	while (i < options.size())
	{
		if (options.at(i) == '+' || options.at(i) == '-')
		{
			sign = options.at(i);
			std::cout << "INDEX : " << i << std::endl;
			i++;
		}
		if (!isModeOption(options.at(i)))
		{
			std::cout << "INDEX : " << i << std::endl;
			return -3;
		}
		if (options.at(i) == 'i')
		{
			sign == '+' ? ch->setInviteMode(1) : ch->setInviteMode(0);
		}
		//ECHAR OJO
		else if (options.at(i) == 'l')
		{
			int limitPos = 0;
			if ((params.size() <= 1 && sign == '+'))
				return -1;
			if (sign == '+')
			{
				limitPos = pos + 1;
				for (size_t i = 0; i != params.at(limitPos).size(); i++)
				{
					if (!std::isdigit(params.at(limitPos).at(i)))
						return -2;
				}
				pos++;
				if (pos >= params.size())
					return -2;
			}
			sign == '+' ? ch->setMaxUsers(std::atoi(params[limitPos].c_str())) : ch->setMaxUsers(-1);
		}
		else if (options.at(i) == 't')
		{
			sign == '+' ? ch->setTopicRestriction(0) : ch->setTopicRestriction(1);
		}
		else if (options.at(i) == 'k')
		{
			std::cout << "GUACAMAYO" << std::endl;
			int	passPos = 0;
			if ((params.size() <= 1  && sign == '+'))
				return -1;
			if (sign == '+')
			{
				passPos = pos + 1;
				pos++;
				if (pos >= params.size())
				{
					std::cout << "eeeeeeeeeJODEER" << std::endl;
					return -2;
				}
			}
			std::cout << "GUACAMAYO --> " << pos << std::endl;
			sign == '+' ? ch->setPassword(params.at(passPos)) : ch->setPassword(" ");
			std::cout << "GUACAMAYO" << std::endl;
		}
		else if (options.at(i) == 'o')
		{
			for (size_t y = i + 1; y < options.size(); y++)
				if (options.at(y) != 'o')
					return -2;
			for (size_t i = 1; i < options.size(); i++)
			{
				for (size_t y = 0; y < ch->getUsers().size(); y++)
				{
					std::vector<std::string*>::iterator it = ch->getUsers().begin() + y;
					if (*(ch->getUsers().at(y)) == params[i])
					{
						ch->getUsers().erase(it);
						ch->getAdmins().push_back(*it);
					}
				}
			}
		}
		i++;
	}
	return 1;
}


int Command::prePlusMode(Channel *ch, std::vector<std::string> params)
{
	size_t	i;
	std::string options(params[0]);


	i = 1;
	while (i < params.size())
	{
		if (params[i].find('+') || params[i].find('-'))
			options += params[i];
		i++;
	}
	std::cout << "OPTIONS : " << options << std::endl;
	return (plusMode(ch, params, options));
}





void	Command::modeInstructions(Channel *ch, std::vector<std::string> params)
{
	std::vector<std::string> neoParams;
	int			status;

	for (size_t i = 2; i < params.size(); i++)
	{
		std::string options(params[i++]);
		if (options[0] != '+' && options[0] != '-')
		{
			markEverything(ERR_UNKNOWNMODE, params);
			launcher->setPollOut(1);
			return ;
		}
		neoParams.push_back(options);
		while (i < params.size()) //&& (params.at(i).at(0) != '+' && params.at(i).at(0) != '-'))
			neoParams.push_back(params.at(i++));
		status = prePlusMode(ch, neoParams);
		if (status == -1) //|| neoParams.size() > 3)
		{
			markEverything(ERR_NEEDMOREPARAMS, params);
			launcher->setPollOut(1);
			return ;
		}
		else if (status == -2 || status == -3)
		{
			std::cout << "STATUS : " << status << std::endl;
			markEverything(ERR_UNKNOWNMODE, params);
			launcher->setPollOut(1);
			return ;
		}
		else if (status == -7)
		{
			markEverything(ERR_USERNOTINCHANNEL, params);
			launcher->setPollOut(1);
			return ;
		}
	}
	std::vector<std::string> target;

	target.push_back(ch->getId());
	markChannelPollOut(channels, target, params, MODE_OK);
	markEverything(MODE_OK, params);
	launcher->setPollOut(1);
}


void	Command::modeOptions(Channel *ch, std::vector<std::string> params)
{
	if (params.size() == 2)
	{
		if (ch->getPassword() != "")
			params.push_back("+k");
		if (ch->getMaxUsers() > 0)
			params.push_back("+l");
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
	/*if (!ch->isAdmin(params[2]) && !ch->isUser(params[2]))
	{
		return msgError(ERR_USERNOTINCHANNEL, params);
	}*/
	modeOptions(ch, params);
}









void	Command::join(const std::vector<std::string>& params, str* userId)
{
	std::cout << "params 1 : " << params[1] << std::endl;
	str name = params[1];
	str password = (params.size() == 3) ? params[2] : "";  
	std::vector<std::string>	channels_name;
	//std::cout << "CHANNEL PSSWD : " << password << std::endl;
	channels_name = split(params[1], ",");

	for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); it++)
	{
		vectorCh::iterator channel = getChannel(*it);
		if (channel == channels->end())
		{
			if ((it)->at(0) != '&' && (it)->at(0) != '#')
				throw std::logic_error("ERR_BADCHANMASK");
			Channel ch = Channel(*it);
			ch.joinClient(userId, password, true);
			std::cout << ch.nameAdmin(*userId) << std::endl;
			channels->push_back(ch);

		}
		else
			channel->joinClient(userId, password, false);
	}
	//markEverything(JOIN_OK, params);
	//launcher->setPollOut(1);
}

vectorCh::iterator	Command::getChannel(const str& id)
{
	vectorCh::iterator channel = channels->begin();
/*	if (channel != channels->end())
	{
		std::cout << "ESTA SEEET??" << std::endl;
		std::cout << channel->getId() << " " << id << std::endl;
	}*/
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
        stringToEnumMap["MODE"] = MODE;
        stringToEnumMap["MODE_WITH_CHANNEL"] = MODE_WITH_CHANNEL;
        stringToEnumMap["ERR_NOTONCHANNEL"] = ERR_NOTONCHANNEL;
        stringToEnumMap["ERR_BADCHANMASK"] = ERR_BADCHANMASK;
        stringToEnumMap["ERR_NEEDMOREPARAMS"] = ERR_NEEDMOREPARAMS;
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

