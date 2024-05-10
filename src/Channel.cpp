#include <Ircsrv.hpp>
#include <Channel.hpp>

Channel::Channel(): users(0), admins(0), topic(""), id(""), password(""), maxUsers(-1), inviteMode(false)
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
	this->users = src.getUsers();
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

int Channel::getMaxUsers() const
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

void	Channel::setMaxUsers(int src)
{
	this->maxUsers = src;
}

void	Channel::setInviteMode(bool src)
{
	this->inviteMode = src;
}

void	Channel::joinClient(const str& userId, const bool& isAdmin)
{
	try
	{
		joinClient(users, userId, isAdmin);
	}
	catch (std::exception& e)
	{
		joinClient(admins, userId, isAdmin);
	}
}
void 	Channel::joinClient(vectorStr& users, const str& userId, const bool& isAdmin)
{
	if (userId == "")
		throw std::logic_error("Provide a valid userId to join to the Channel.");
	if (users.size() == maxUsers)
		throw std::logic_error("The Channel is full");
	vectorStr::iterator user = findUser(users, userId);
	if (user != users.end())
		throw std::logic_error("The user is already in the Channel");
	users.push_back(userId);
}

void	Channel::deleteClient(vectorStr& users, const str& userId)
{
	vectorStr::iterator user = findUser(users, userId);
	if (user == users.end())
		throw std::logic_error("The userId wasn't find in the Channel");
	users.erase(user);
}

void	Channel::deleteClient(const str& userId)
{
	try
	{
		deleteClient(users, userId);
	}
	catch(std::exception& e)
	{
		deleteClient(admins, userId);
	}
}

vectorStr::iterator	Channel::findUser(vectorStr& users, const str& userId) const
{
	vectorStr::iterator user = users.begin();
	while (user != users.end() && *user != userId)
		user++;
	return user;
}
