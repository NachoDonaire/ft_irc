#include <Ircsrv.hpp>
#include <Channel.hpp>

Channel::Channel(): users(0), admins(0), topic(""), id(""), password(""), maxUsers(-1), inviteMode(false), topicRestriction(false)
{
}

Channel::Channel(const Channel& src)
{
	*this = src;
}

Channel::Channel(const std::string id): users(0), admins(0), topic(""), password(""), inviteMode(false), topicRestriction(false)
{
	this->setId(id);
	maxUsers = -1;

}

Channel::~Channel()
{
}

Channel& Channel::operator = (const Channel& src)
{
	this->users = src.getUsers();
	setAdmins(src.getAdmins());
	setTopic(src.getTopic());
	setId(src.getId());
	setPassword(src.getPassword());
	setMaxUsers(src.getMaxUsers());
	setInviteMode(src.getInviteMode());
	setTopicRestriction(src.getTopicRestriction());
	return *this;
}

vectorStr &Channel::getUsers(void) 
{
	return this->users;
}

vectorStr &Channel::getAdmins() 
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
	if (parseChannelId(src) != 1)
		throw std::logic_error("ERR_BADCHANNELID");
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

void	Channel::joinClient(std::string &userId, const std::string password, const bool& isAdmin)
{
	if (password != this->password)
		throw std::logic_error("ERR_BADCHANNELKEY");
	if (!isAdmin)
		joinClient(users, userId, isAdmin);
	else
		joinClient(admins, userId, isAdmin);
}

void	Channel::printAdminUsers() const
{

	std::vector<std::string *>::const_iterator user = users.begin();
	while (user != users.end())
	{
		std::cout << **user << std::endl;
		user++;
	}
	std::cout << "admins: ";
	vectorStr::const_iterator admin = admins.begin();
	while (admin != this->admins.end())
	{
		std::cout << **admin << std::endl;
		admin++;
	}
	std::cout << std::endl;
}

void	Channel::setGuest(std::string *name)
{
	for (std::vector<std::string*>::iterator it = invitations.begin(); it != invitations.end(); it++)
	{
		if (**it == *name)
			return ;
	}
	invitations.push_back(name);
}

bool	Channel::isInvited(std::string &userId)
{
	std::vector<int> del;
	bool		rtr;

	rtr = 0;
	for (size_t i = 0; i < invitations.size(); i++)
	{
		if (userId == *invitations.at(i))
		{
			del.push_back(i);
			rtr = 1;
		}
	}
	for (size_t i = 0; i < del.size(); i++)
	{
		std::vector<std::string *>::iterator guest = invitations.begin() + del.at(i);
		invitations.erase(guest);
	}
	return rtr;
}

			


void 	Channel::joinClient(vectorStr& users, std::string &userId, const bool& isAdmin)
{
	if (inviteMode == true && !isInvited(userId))
		throw std::logic_error("ERR_INVITEONLYCHAN");
	if (userId == "")
		throw std::logic_error("ERR_NEEDMOREPARAMS");
	if (users.size() + admins.size() >= maxUsers && maxUsers >= 0)
		throw std::logic_error("ERR_CHANNELISFULL");
	isAdmin ? admins.push_back(&userId) : users.push_back(&userId);
}

void	Channel::deleteClient(vectorStr& users, const std::string* userId)
{
	vectorStr::iterator user = findUser(users, userId);
	if (user == users.end())
		user = findUser(admins, userId);
	if (user == admins.end())
		throw std::logic_error("The userId wasn't find in the Channel");
	users.erase(user);
}

void	Channel::deleteClient(const std::string* userId)
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

vectorStr::iterator	Channel::findUser(vectorStr& users, const std::string* userId)
{
	vectorStr::iterator user = users.begin();
	while (user != users.end() && **user != *userId)
	{
		user++;
	}
	return user;
}

void	Channel::checkInvite(const std::string* senderId)
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

void	Channel::changeTopic(const std::string* userId, const std::string value)
{
	vectorStr::iterator user = findUser(admins, userId);
	if (topicRestriction == true && user == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	else if (topicRestriction == false && user == admins.end() && findUser(users, userId) == users.end())
		throw std::logic_error("The user is not in the Channel.");
	this->topic = value;
}

void	Channel::changeInviteMode(const std::string* userId, const bool& value)
{
	if (findUser(admins, userId) == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	this->inviteMode = value;
}

void	Channel::changePassword(const std::string* userId, const std::string newPassword)
{
	if (findUser(admins, userId) == admins.end())
		throw std::logic_error("The user need to be an operator of the Cannel.");
	this->password = newPassword;
}

bool	Channel::isRegister(std::string* userId)
{
	vectorStr::iterator user = findUser(users, userId);
	if (user == users.end())
		user = findUser(admins, userId);
	return (user != admins.end());
}

std::string	Channel::nameAdmin(std::string admin)
{
	for (std::vector<std::string *>::iterator i = admins.begin(); i != admins.end(); i++)
	{
		if (admin == **i)
			return ("@" + **i);
	}
	return ("");
}

int	Channel::parseChannelId(str src) const
{
	size_t	pos;

	pos = 0;
	while (pos < src.size())
	{
		if (src[pos] == '\a' || src[pos] == '\b' || src[pos] == '\r' || src[pos] == '\n')
			return (0);
		pos++;
	}
	return (1);
}

bool	Channel::isAdmin(std::string nick)
{
	for (vectorStr::iterator it = admins.begin(); it != admins.end(); it++)
	{
		if (nick == **it)
		{
			return 1;
		}
	}
	return 0;
}

bool	Channel::isUser(std::string nick)
{
	for (vectorStr::iterator it = users.begin(); it != users.end(); it++)
	{
		if (nick == **it)
			return 1;
	}
	return 0;
}
