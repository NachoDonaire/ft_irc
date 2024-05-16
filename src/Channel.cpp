#include <Ircsrv.hpp>
#include <Channel.hpp>

Channel::Channel(): users(0), admins(0), topic(""), id(""), password(""), maxUsers(-1), inviteMode(false), topicRestriction(false)
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

bool	Channel::getTopicRestriction() const
{
	return this->topicRestriction;
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

void	Channel::setTopicRestriction(const bool& src)
{
	this->topicRestriction = src;
}

void	Channel::joinClient(const str& userId, const str& password, const bool& isAdmin)
{
	std::cout << "llega hasta aqui y mete al usuario" << std::endl;
	if (password != this->password)
		throw std::logic_error("The password provided is not valid for the channel");
	if (!isAdmin)
		joinClient(users, userId, isAdmin);
	else
		joinClient(admins, userId, isAdmin);
}
void 	Channel::joinClient(vectorStr& users, const str& userId, const bool& isAdmin)
{
	if (userId == "")
		throw std::logic_error("Provide a valid userId to join to the Channel.");
	if (users.size() + admins.size() >= maxUsers && maxUsers >= 0)
		throw std::logic_error("The Channel is full");
	vectorStr::iterator user = findUser(users, userId);
	if (user != users.end() || findUser(admins, userId) != admins.end())
		throw std::logic_error("The user is already in the Channel");

	isAdmin ? admins.push_back(userId) : users.push_back(userId);
	std::cout << "llega hasta aqui y mete al usuario" << std::endl;
}

void	Channel::deleteClient(vectorStr& users, const str& userId)
{
	vectorStr::iterator user = findUser(users, userId);
	if (user == users.end())
		user = findUser(admins, userId);
	if (user == admins.end())
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

void	Channel::checkInvite(const str& senderId)
{
	vectorStr::iterator sender = findUser(admins, senderId);
	if (sender == admins.end())
		throw std::logic_error("The sender is not an operator of the Channel.");
	vectorStr::iterator user = findUser(admins, senderId);
	if (inviteMode == true && user == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	else if (inviteMode == false && user == admins.end() && findUser(users, senderId) == users.end())
		throw std::logic_error("The user is not in the Channel.");
	//if the invite can be done, the function doesn't throw any exception
}

void	Channel::changeTopic(const str& userId, const str& value)
{
	vectorStr::iterator user = findUser(admins, userId);
	if (topicRestriction == true && user == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	else if (topicRestriction == false && user == admins.end() && findUser(users, userId) == users.end())
		throw std::logic_error("The user is not in the Channel.");
	this->topic = value;
}

void	Channel::changeInviteMode(const str& userId, const bool& value)
{
	if (findUser(admins, userId) == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	this->inviteMode = value;
}

void	Channel::changePassword(const str& userId, const str& newPassword)
{
	if (findUser(admins, userId) == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	this->password = newPassword;
}

bool	Channel::isRegister(str& userId)
{
	vectorStr::iterator user = findUser(users, userId);
	if (user == users.end())
		user = findUser(admins, userId);
	return (user != admins.end());
}
