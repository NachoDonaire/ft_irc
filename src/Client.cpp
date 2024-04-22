#include <Client.hpp>

std::vector<std::string> Client::split(std::string na, char c)
{
	std::vector<std::string> tokens;
	std::string		 token;
	std::string		n = na;
	size_t pos;

	if (n.find(c) == std::string::npos)
	{
		tokens.push_back(na);
	}
	while ((pos = n.find(c)) != std::string::npos)
	{
		token = n.substr(0, pos);
		tokens.push_back(token);
		n.erase(0, pos + 1);
	}
	return tokens;
}




Client::Client(int socket, int i) : fd(socket), id(i)
{
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
	return 1;
}

void	Client::setNick(std::string n)
{
	this->nick = n;
}

void	Client::setPsswd(std::string p)
{
	this->psswd = p;
}

int Client::parseMsg()
{
	std::vector<std::string> tokens;
	size_t i;
	bool        error;

	error = 0;
	i = 0;
	tokens = split(this->msg, ' ');
	//std::cout << "HOLA" << std::endl;
	//std::cout << tokens[0] << tokens.size() << std::endl;
	while (i < tokens.size())
	{
		if (i == 0)
		{
		//	std::cout << "wwweee" << std::endl;
			error = cmdAnalyzer(tokens[i]);
		}
		if (error == 0)
		{
		//	std::cout << "www1111" << std::endl;
			params.push_back(tokens[i]);
		}
		else if (error == 1)
		{
		//	std::cout << "www222" << std::endl;
			return 1;
		}

		i++;
	}
	//if (psswd != servPsswd)
	//	return 2;
	return 0;
}



int Client::pass()
{
	if (params.size() != 2)
		return 3;
	setPsswd(params[1]);
	return (0);
}
