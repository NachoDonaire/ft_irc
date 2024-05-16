#include <Ircsrv.hpp>
#include <Client.hpp>

/*
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
*/
int	Client::getParseStatus()
{
	return this->parseStatus;
}


Client::Client(int socket, int i, std::string sp, std::string hn) : fd(socket), id(i), servPsswd(sp), hostName(hn)
{
	parseStatus = 1;
	off = 0;
	registered = 0;
	msg = "";
	channels = vectorStr(0);
	//pollout = 1;
}

void	Client::setMsg(std::string message)
{
	this->msg = message;
}

/*
void	Client::setParams(std::map<int, std::vector<std::string> > p)
{
	this->params = p;
}
*/

void	Client::printShait()
{
	std::cout << "responses : " << std::endl;
	for (size_t i = 0; i < responses.size(); i++)
	{
		std::cout << responses.at(i) << std::endl;
	}
	std::cout << "cmd : " << std::endl;
	for (std::map<int, std::vector<std::string> >::iterator it = cmd.begin(); it != cmd.end(); it++)
	{
		std::cout << "this is client cmd second vector" << std::endl;
		for (size_t i = 0; i < it->second.size() ; i++)
			std::cout << it->second.at(i) << std::endl;
	}
}

Client::Client()
{
}


Client::~Client()
{
	//std::cout << "Cliente deleteado" << std::endl;
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

Client* Client::operator=(const Client* f)
{
	fd = f->fd;
	servPsswd = f->servPsswd;
	hostName = f->hostName;
	off = f->off;
	registered = f->registered;
	status = f->status;
	pollout = f->pollout;
	id = f->id;
	parseStatus = f->parseStatus;
	msg = f->msg;
	cmd = f->cmd;
	psswd = f->psswd;
	nickname = f->nickname;
	username = f->username;
	responses =  f->responses;
	cmds = f->cmds;
	channels = f->channels;
	return this;
}



void	Client::setNick(std::string n)
{
	this->nickname = n;
}

void	Client::setPsswd(std::string p)
{
	this->psswd = p;
}

void	Client::printCmd()
{
	std::cout << "alla va cmd mapa" << std::endl;
	for (std::map<int, std::vector<std::string> >::iterator it = cmd.begin(); it != cmd.end(); it++)
	{
		std::cout << (*it).second[0] << std::endl;
	}
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
		/*if (parameters[0] == "NICK")
		{
			//std::cout << "EEO" << std::endl;
			std::cout << msg << std::endl;
		}*/
		cmd[i] = parameters;
		parameters.clear();
	}
	//printCmd();
	return (0);
}
void	Client::printeito()
{
	std::cout << "pass : " << psswd << std::endl;
	std::cout << "nick : " << nickname << std::endl;
	std::cout << "user : " << username << std::endl;
	std::cout << "socket : " << fd << std::endl;
}

vectorStr	Client::getChannels() const
{
	return this->channels;
}

void		Client::setChannels(const vectorStr& newChannels)
{
	this->channels = newChannels;
}

/*
void		Client::join(const str& channelId)
{
}

void		Client::kick(const str& channelId)
{
}
*/
