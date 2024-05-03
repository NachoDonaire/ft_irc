#include <Client.hpp>

std::string	Client::msgGenerator(int msg, std::vector<std::string> params)
{
	std::string response(hostName);

	if (msg == 421)
	{
		response += " 421 " + params[0] + " :Unknown command\r\n";
	}
	else if (msg == 381)
	{
		response += " 381 " + getNick() + " : Capabilities displayed properly !\r\n";
	}
	else if (msg == -1)
	{
		response += " NICK :" + params[1] + "\r\n";
	}
	else if (msg == -2)
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
	else if (msg == 1)
	{
		response += " 001 " + getNick() + " :Welcome to ircserv !\r\n";
	}
	if (msg == 461)
	{
		response += " 461 ";
		response += params[0];
		response += " :Not enough or too much params for ";
		response += params[0];
		response += "\r\n";
	}

	return (response);
}



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
		std::string notValidCmd = msgGenerator(461, params);
		send(fd, notValidCmd.c_str(), notValidCmd.size(), 0);
	}
}

void	Client::cap(std::vector<std::string> params)
{
	std::string response(hostName);

	if (params[1] == "LS")
		response += " CAP * LS :kajuto por aqui, kajuto por alla\r\n";
	else if (params[1] == "END")
		response = msgGenerator(381, params);


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
		return (2);
	}
	tokens = split(msg, "\r\n");
	for (size_t i = 0; i < tokens.size(); i++)
	{
	
		parameters = split(tokens[i], " ");
		cmd[i] = parameters;
		parameters.empty();
	}
	return (0);
}

void	Client::notEnoughParams(std::vector<std::string> params)
{
	std::string msg = msgGenerator(461, params);
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

//queda por ver que el user es unico
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

//queda por comprobar que el nick es unico
int	Client::nick(std::vector<std::string> params)
{
	std::string response = msgGenerator(-1, params);

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
	std::string msg = msgGenerator(-2, params);
	send(fd, msg.c_str(), msg.size(), 0);
	close(fd);
}

int Client::welcome()
{
	std::vector<std::string> tuke;
	if (psswd == "" || nickname == "" || username == "")
		return -1;
	std::string msg = msgGenerator(1, tuke);
	std::string error(hostName);
	if (psswd == servPsswd)
	{
		send(fd, msg.c_str(), msg.size(), 0);
	}
	else
	{
		send(fd, error.c_str(), error.size(), 0);
		close(fd);
		return 2;
	}
	return (0);
}
