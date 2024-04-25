#include <Client.hpp>

void	Client::handleCmd()
{
	for (std::map<int, std::vector<std::string> >::iterator i = cmd.begin(); i != cmd.end(); i++)
	{
		std::vector<std::string> params;
		for (std::vector<std::string>::iterator y = i->second.begin(); y != i->second.end(); y++)
		{
			params.push_back(*y);
		}
		launchAction(params);
		params.empty();
	}
}

void	Client::launchAction(std::vector<std::string> params)
{
	std::string cmd = params[0];
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
	else if (cmd == "")
		return ;
	else
	{
		std::string notValidCmd("Not valid cmd\r\n");
		send(fd, notValidCmd.c_str(), notValidCmd.size(), 0);
	}
}

void	Client::cap(std::vector<std::string> params)
{
	std::string response(hostName);

	if (params[1] == "LS")
		response += " CAP * LS :kajuto por aqui, kajuto por alla\r\n";
	else if (params[1] == "END")
		response += " 381 " + getNick() + " : Capabilities displayed properly !\r\n";


	send(fd, response.c_str(), response.size(), 0);
}


std::vector<std::string> Client::split(std::string na, const char *c)
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

int	Client::getParseStatus()
{
	return this->parseStatus;
}


Client::Client(int socket, int i, std::string sp, std::string hn) : fd(socket), id(i), servPsswd(sp), hostName(hn)
{
	parseStatus = 1;
	off = 0;
}

void	Client::setMsg(std::string message)
{
	this->msg = message;
}

Client::Client()
{
}


Client::~Client()
{
}

int Client::cmdAnalyzer(std::string cmd)
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

void	Client::setNick(std::string n)
{
	this->nickname = n;
}

void	Client::setPsswd(std::string p)
{
	this->psswd = p;
}

int Client::parseMsg()
{
	std::vector<std::string> tokens;
	std::vector<std::string> parameters;

	if (msg.find("\r\n") == std::string::npos || msg.size() >= 512)
	{
		//std::cout << msg.size() << std::endl;
		return (2);
	}
	tokens = split(msg, "\r\n");
	for (size_t i = 0; i < tokens.size(); i++)
	{
	
		parameters = split(tokens[i], " ");
		cmd[i] = parameters;
		parameters.empty();
	}
	/*std::cout << "stored" << std::endl;
	for (std::map<int, std::vector<std::string > >::iterator i = cmd.begin(); i != cmd.end(); i++)
	{
		std::cout << i->first;
	       	std::cout << 	" : " ;
		//std::cout << i->second.size();
		for (std::vector<std::string>::iterator it = i->second.begin(); it != i->second.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}*/
	return (0);
}

void	Client::notEnoughParams(std::vector<std::string> params)
{
	std::string msg(hostName);

	msg += " 461 ";
	msg += params[0];
	msg += " :Not enough or too much params for ";
	msg += params[0];
	msg += "\r\n";


	send(fd, msg.c_str(), msg.size(), 0);
}

int	Client::pass(std::vector<std::string> params)
{
	if (params.size() != 2)
	{
		notEnoughParams(params);
		return 3;
	}
	setPsswd(params[1]);
	//welcome(params);
	return 0;
}

int	Client::user(std::vector<std::string> params)
{
	if (params.size() < 5)
	{
		notEnoughParams(params);
		return 3;
	}
	setUser(params[1]);
	welcome();
	return 0;
}

int	Client::nick(std::vector<std::string> params)
{
	std::string response(hostName);

	response += " NICK :" + params[1] + "\r\n";
	if (params.size() != 2)
	{
		notEnoughParams(params);
		return 3;
	}
	nickname = params[1];

	send(fd, response.c_str(), response.size(), 0);
	return 0;
}

void	Client::quit(std::vector<std::string> params)
{
	off = 1;
	std::string msg(hostName);
	if (params.size() <= 1)
		msg += " QUIT: No leaving message";
	else
	{
		std::string quitMsg(&params[1].c_str()[1]);
		std::cout << "eeepa " << quitMsg << std::endl;
		msg += " QUIT: ";
		msg += quitMsg;
	}

	send(fd, msg.c_str(), msg.size(), 0);
	close(fd);
}

int Client::welcome()
{
	if (psswd == "" || nickname == "" || username == "")
		return -1;
	std::string msg(hostName);
	std::string error(hostName);
	
	/*msg += getUser();
	error += getUser();
	msg += " :Welcome to ircserv !\r\n";
	error += " :Not valid password\r\n";
	*/
	//if (params.size() != 2)
	//{
	//	notEnoughParams(params);
	//	return 3;
	//}
	///setPsswd(params[1]);
	msg += " 001 " + getNick() + " :Welcome to ircserv !\r\n";
	if (psswd == servPsswd)
	{
		send(fd, msg.c_str(), msg.size(), 0);
	}
	else
	{
		send(fd, error.c_str(), error.size(), 0);
		close(fd);
		//quit();
		return 2;
	}
	return (0);
}
