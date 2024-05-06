#include <Channel.hpp>

Channel::Channel()
{

}

Channel::Channel(Client op, std::string t)
{
	operators.push_back(op);
	clients.push_back(op);
	if (t == "")
		return ;
	topic = t;
}

Channel::~Channel()
{
}

