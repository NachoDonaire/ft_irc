#include <Command.hpp>

Command::Command()
{}

Command::Command(Client *l, std::vector<Client *> cl, std::string hostN, std::string ms, std::string sp, std::map<int, std::vector<std::string> > cm) : msg(ms)
{

	for (size_t i = 0; i < cl.size(); i++)
	{
		clients.push_back(cl[i]);
	}
	servPsswd = sp;
	hostName = hostN;
	launcher = l;
	cmd = cm;
	//launcher->setParams(cm);
}

std::string	Command::msgGenerator(int msg, std::vector<std::string> params)
{
	std::string response(hostName);

	//std::cout << c.getStatus() << std::endl;
	if (msg == CMD_NOTFOUND)
	{
		response += " 421 " + params[0] + " :Unknown command\r\n";
	}
	else if (msg == NICK_OK)
	{
		response += " 381 " + launcher->getNick() + " :Nick " + launcher->getNick() + " magnificent set!\r\n";
	}
	else if (msg == NICK_REPEATED)
	{
		response += " 433 * " + params[1] + ":Nickname is already in use.\r\n";
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
	else if (msg == WELCOME && launcher->getStatus() == REGISTER_PENDING)
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
		////////std::cout << "weepa" << std::endl;
		std::cout << launcher->getNick() << std::endl;
		response += " 401 " + launcher->getNick() +  " ";
		for (size_t  i = 2; i < params.size(); i++)
		{
			response += params[i];
			response += " ";
		}
		response += "\r\n";
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

void	Command::notEnoughParams(std::vector<std::string> params)
{
	(void)params;
	//std::string msg = msgGenerator(461, params);
	send(launcher->getSocket(), msg.c_str(), msg.size(), 0);
}

void	Command::handleCmd()
{	
	std::vector<std::string> params;
	for (std::map<int, std::vector<std::string> >::iterator i = cmd.begin(); i != cmd.end(); i++)
	{
		for (std::vector<std::string>::iterator y = i->second.begin(); y != i->second.end(); y++)
		{
			params.push_back(*y);
			if (params[0] == "NICK")
				std::cout << *y << std::endl;
		}
		markAction(params);
		params.clear();
	}
}

void	Command::markAction(std::vector<std::string> params)
{
	std::string cmd = params[0];
	std::cout << cmd << " : " << params.size()<< std::endl;;
	//std::cout << "iusa" << std::endl;
	//std::cout << cmd << std::endl;
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
	else if (cmd == "PRIVMSG")
	{
		std::cout << "comienza" << std::endl;
		privmsg(params);
	}
	else if (cmd == "")
		return ;
	else
	{
		launcher->setStatus(CMD_NOTFOUND);
	}
}

void	Command::cap(std::vector<std::string> params)
{
	std::string response;

	if (params[1] == "LS")
		launcher->setCommand(CAP_LS);
	else if (params[1] == "END")
		launcher->setCommand(CAP_END);
	
	response = msgGenerator(launcher->getCommand(), params);
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

int	Command::pass(std::vector<std::string> params)
{
	std::string response;

	if (params.size() != 2)
	{
		launcher->setStatus(ERROR);
		launcher->setCommand(CMD_ERROR);
		response = msgGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
		launcher->setPollOut(1);
		return 3;
	}
	launcher->setPsswd(params[1]);
	if (launcher->getNick() == "" || launcher->getUser() == "" || launcher->getPsswd() == "")
	{
		launcher->setStatus(REGISTER_PENDING);
	}
	else
	{
		launcher->setStatus(REGISTERED);
	}
	if (launcher->getPsswd() != servPsswd)
		launcher->setCommand(BAD_PSSWD);
	else
		launcher->setCommand(OK_PSSWD);
	response = msgGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
	launcher->setPollOut(1);
	//welcome();
	return 0;
}

int	Command::user(std::vector<std::string> params)
{
	std::string response;
	if (params.size() < 5)
	{
		launcher->setStatus(ERROR);
		response = msgGenerator(launcher->getCommand(), params);
		launcher->setUser(params[1]);
		launcher->setPollOut(1);
		return 3;
	}
	if (launcher->getNick() == "" || launcher->getUser() == "" || launcher->getPsswd() == "")
	{
		launcher->setStatus(REGISTER_PENDING);
	}
	else
		launcher->setStatus(REGISTERED);
	response = msgGenerator(launcher->getCommand(), params);
	launcher->setUser(params[1]);
	launcher->setPollOut(1);
	welcome(params);
	return 0;
}

//queda por comprobar que el nick es unico
int	Command::nick(std::vector<std::string> params)
{
	//std::string response = msgGenerator(-1, params);
	std::string response;

	if (params.size() != 2)
	{
		launcher->setCommand(CMD_ERROR);
		response = msgGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
		launcher->setPollOut(1);
		return 3;
	}
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getNick() == params[1])
		{
			launcher->setCommand(NICK_REPEATED);
			response = msgGenerator(launcher->getCommand(), params);
			launcher->setResponse(response);
			launcher->setNCmd(launcher->getCommand());
			launcher->setPollOut(1);
			return 2;
		}
	}
	launcher->setNick(params[1]);
	if (launcher->getNick() == "" || launcher->getUser() == "" || launcher->getPsswd() == "")
	{
		launcher->setStatus(REGISTER_PENDING);
	}
	else
		launcher->setStatus(REGISTERED);
	launcher->setCommand(NICK_OK);
	response = msgGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
	launcher->setPollOut(1);
	//welcome();
	return 0;
}

void	Command::quit(std::vector<std::string> params)
{
	launcher->setOff(1);
	std::string response;
	//std::string msg = msgGenerator(-2, params);
	//send(launcher->getSocket(), msg.c_str(), msg.size(), 0);
	launcher->setCommand(QUIT);
	response = msgGenerator(launcher->getCommand(), params);
	launcher->setResponse(response);
	launcher->setNCmd(launcher->getCommand());
	launcher->setPollOut(1);
	//close(launcher->getSocket());
}

int Command::welcome(std::vector<std::string> params)
{
	std::string response;

	if (launcher->getStatus() == REGISTERED)
	{
		std::cout << "registered" << std::endl;
	}
	else if (launcher->getStatus() == REGISTER_PENDING)
	{
		std::cout << "NOT registered" << std::endl;
		launcher->setCommand(WELCOME);
		response = msgGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
	}
	return (0);
}

void	Command::markie(Client *c, std::vector<std::string> params)
{
	std::string response;

	c->setCommand(PRIVMSG);
	response = msgGenerator(c->getCommand(), params);
	c->setResponse(response);
	c->setNCmd(c->getCommand());
	c->setPollOut(1);
}

void	Command::markPollOut(std::vector<Client *> clients, std::vector<std::string> dest, std::vector<std::string> params)
{
	for (size_t y = 0; y < dest.size(); y++)
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (dest[y] == clients[i]->getNick())
			{
				std::cout << "eing??" << std::endl;
				markie(clients[i], params);
			}
		}
	}
}

void	Command::privmsg(std::vector<std::string> params)
{
	std::vector<std::string>	dest;
	std::string			response;

	//"PRIVMSG" "tucson,jossie,esmeraldo" "hola!!!!!" "iuui"
	if (params.size() < 3)
	{
		//caso de error
		launcher->setCommand(CMD_ERROR);
		response = msgGenerator(launcher->getCommand(), params);
		launcher->setResponse(response);
		launcher->setNCmd(launcher->getCommand());
		launcher->setPollOut(1);
		return ;
	}
	//caso respuesta valida
	dest = split(params[1], ",");
	markPollOut(clients, dest, params);
	std::cout << "gepasa" << std::endl;
}

