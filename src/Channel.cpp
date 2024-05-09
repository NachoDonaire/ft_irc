#include <Ircsrv.hpp>
#include <Channel.hpp>

Channel::Channel(): users(0), admins(0), topic(""), id(""), password(""), maxUsers(0), inviteMode(false)
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
	users = src.getUsers();
	return *this;
}

vectorStr Channel::getUsers(void) const
{
	return this->users;
}

vectorStr Channel::getAdmins() const
{
	return this->admins;
}

str	Channel::getTopic() const
{
	return this->topic;
}

str	Channel::getId() const
{
	return this->id;
}

str	Channel::getPassword() const
{
	return this->password;
}

unsigned int Channel::getMaxUsers() const
{
	return this->maxUsers;
}

bool	Channel::getInviteMode() const
{
	return this->inviteMode;
}

void 	Channel::setUsers(vectorStr src)
{
	this->users = src;
}

void	Channel::setAdmins(vectorStr src)
{
	this->admins = src;
}

void	Channel::setTopic(str src)
{
	this->topic = src;
}

void	Channel::setId(str src)
{
	this->id = src;
}

void	Channel::setPassword(str src)
{
	this->password = src;
}

void	Channel::setMaxUsers(unsigned int src)
{
	this->maxUsers = src;
}

void	Channel::setInviteMode(bool src)
{
	this->inviteMode = src;
}


