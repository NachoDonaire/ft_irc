#include <Client.hpp>

std::vector<std::string> Client::split(std::string n, char c)
{
	std::vector<std::string> tokens;
	std::string		 token;
	int			 pos;

	while ((pos = n.find(c)) != std::string::npos)
	{
		token = n.substr(0, pos);
		tokens.push_back(token);
		n.erase(0, pos + 1);
	}
	return tokens;
}




Client::Client(int socket, std::string psswdServ) : fd(socket), servPsswd(psswdServ) 
{
	msg = "";
	psswd = "";
}

void	Client::setMsg(std::string message)
{
	msg = message;
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



int Client::parseMsg()
{
	std::vector tokens;
	int	    i;
	bool        error;

	error = 0;
	i = 0;
	tokens = split(this->msg, ' ');
	while (i < tokens.size())
	{
		if (i == 0)
			error = cmdAnalyzer(tokens[i]);
		if (error != 0)
			params.push_back(tokens[i]);
		else if (error == 1)
			return 1;

		i++;
	}
	if (psswd != servPsswd)
		return 2;
	return 0;
}

	
