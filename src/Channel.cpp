#include <Channel.hpp>

Channel::Channel()
{
//Const
}

Channel::Channel(const Channel& src)
{
//Copy Const
	*this = src;
}

Channel::~Channel()
{
//Dest
}

Channel& Channel::operator = (const Channel& src)
{
	return *this;
}
