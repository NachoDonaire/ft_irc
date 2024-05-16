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

	if (msg == CMD_NOTFOUND)
	{
		response += " 421 " + params[0] + " :Unknown command\r\n";
	}
	else if (msg == OK_PSSWD)
	{
		response += "";
	}
	else if (msg == NICK_OK)
	{
		response += " 381 " + launcher->getNick() + " :Nick " + launcher->getNick() + " magnificent set!\r\n";
	}
	else if (msg == NICK_REPEATED)
	{
		response += " 433 * " + params[1] + " :Nickname is already in use.\r\n";
	}
	else if (msg == QUIT)
	{
		if (params.size() <= 1)
			response += " QUIT: No leaving message";
		else
		{
			std::string quitMsg(&params[1].c_str()[1]);
			response += " QUIT: ";
			response += quitMsg;
		}
	}
	else if (msg == WELCOME)// && launcher->getStatus() == REGISTER_PENDING)
	{
		response += " 001 " + launcher->getNick() + " :Welcome to ircserv !\r\n";
	}
	else if (msg == ERROR)
	{
		response += " 461 ";
		response += params[0];
		response += " :Not enough or too much params for ";
		response += params[0];
		response += "\r\n";
	}
	else if (msg == BAD_PSSWD)
	{
		response += " Bad password, try to reconnect setting password again\r\n";
	}
	else if (msg == PRIVMSG)
	{
		//std::cout << "weepa" << std::endl;
		//std::cout << launcher->getNick() << std::endl;
		response = ":";
		response += launcher->getNick();
		response += " PRIVMSG " + params[1] +  " :";
		for (size_t  i = 2; i < params.size(); i++)
		{
			response += params[i];
			response += " ";
		}
		response += "\r\n";
		//std::cout << response << std::endl;
		//std::cout << "endofweeepa" << std::endl;
	}
	//std::cout << "cmdResponse: " << response << std::endl;
	return (response);
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
		params.clear();
	}
}

void	Command::markAction(std::vector<std::string> params)
{
	std::string cmd = params[0];
	std::cout << "Params: {" << std::endl;
	for (std::size_t i = 0; i < params.size(); i++)
	{
		std::cout << " " << params[i];
	}
	std::cout << " }" << std::endl;
	//std::cout << cmd << " : " << params.size()<< std::endl;;
	//std::cout << "iusa" << std::endl;
	//std::cout << cmd << std::endl;
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
	else if (cmd == "PRIVMSG")
	{
		//std::cout << "comienza" << std::endl;
		privmsg(params);
	}
	else if (cmd == "JOIN")
	{
		std::cout << "el comando es join" << std::endl;
		std::cout << "El nick de la clase cliente que ejecuta el join es: " << launcher->getUser() << std::endl;
		join(params, launcher->getUser());
	}
	else if (cmd == "")
		return ;
	else
	{
		markEverything(CMD_NOTFOUND, params);
		launcher->setPollOut(1);
		//launcher->setCommand(CMD_NOTFOUND);
	}
/*
*/
}

void	Command::cap(std::vector<std::string> params)
{
	std::string response;

	if (params[1] == "LS")
		launcher->setCommand(CAP_LS);
	else if (params[1] == "END")
		launcher->setCommand(CAP_END);
	
	response = responseGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
	launcher->setPollOut(1);
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

int Command::cmdAnalyzer(std::string cmd)
{
	if (cmd == "PASS")
		return 0;
	else if (cmd == "CAP")
		return 0;
	else if (cmd == "LS")
		return 0;
	else if (cmd == "NICK")
		return 0;
	else if (cmd == "USER")
		return 0;
	return 1;
}

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
	
		parameters = split(tokens[i], " ");
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
	response = responseGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
}



int	Command::pass(std::vector<std::string> params)
{
	std::string response;

	if (params.size() != 2)
	{
		markEverything(CMD_ERROR, params);
		return 3;
	}
	launcher->setPsswd(params[1]);
	if (launcher->getPsswd() != servPsswd)
		markEverything(BAD_PSSWD, params);
	else
		markEverything(OK_PSSWD, params);
	welcome(params);
	return 0;
}

int	Command::user(std::vector<std::string> params)
{
	std::string response;
	if (params.size() < 5)
	{
		markEverything(ERROR, params);
		return 3;
	}
	launcher->setUser(params[1]);
	markEverything(USER, params);
	welcome(params);
	return 0;
}

int	Command::nick(std::vector<std::string> params)
{
	std::string response;

	if (params.size() != 2)
	{
		markEverything(CMD_ERROR, params);
		return 3;
	}
	for (std::vector<Client >::iterator it = clients->begin(); it != clients->end(); it++)
	{
		if ((*it).getNick() == params[1])
		{
			markEverything(NICK_REPEATED, params);
			launcher->setPollOut(1);
			launcher->setNick("");
			return 2;
		}
	}
	launcher->setNick(params[1]);
	markEverything(NICK_OK, params);
	launcher->setPollOut(1);
	welcome(params);
	return 0;
}

void	Command::quit(std::vector<std::string> params)
{
	launcher->setOff(1);
	markEverything(QUIT, params);
	
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

void	Command::markie(Client *c, std::vector<std::string> params)
{
	std::string response;

	c->setCommand(PRIVMSG);
	response = responseGenerator(c->getCommand(), params);
	c->setResponse(response);
	c->setNCmd(c->getCommand());
	c->setPollOut(1);
}

void	Command::markPollOut(std::vector<Client> *clients, std::vector<std::string> dest, std::vector<std::string> params)
{
	for (size_t y = 0; y < dest.size(); y++)
	{
		for (size_t i = 0; i < clients->size(); i++)
		{
			if (dest[y] == clients->at(i).getNick())
			{
				markie(&clients->at(i), params);
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
		//caso de error
		launcher->setCommand(CMD_ERROR);
		response = responseGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
		launcher->setPollOut(1);
		return ;
	}
	//caso respuesta valida
	dest = split(params[1], ",");
	markPollOut(clients, dest, params);
}

void	Command::join(const vectorStr& params, const str& userId)
{
	std::cout << "params 1 : " << params[1] << std::endl;
	vectorCh::iterator channel = getChannel(params[1]);
	if (channel == channels->end())
	{
		Channel ch = Channel(params[1]);
		ch.joinClient(userId, "", true);
		channels->push_back(ch);
	}
}

vectorCh::iterator	Command::getChannel(const str& id)
{
	vectorCh::iterator channel = channels->begin();
	while (channel != channels->end() && channel->getId() != id)
		++channel;
	return channel;
}
