#ifndef CHANNEL_H
#define CHANNEL_H
#include <iostream>
#include <string>
typedef std::vector<std::string *> 	vectorStr;
typedef std::string  			str;
#define MAX_USERS_PER_CHANNEL		23

class Channel
{
public:
	Channel();
	~Channel();
   	Channel(const Channel& src);
	Channel(const std::string id);
   	Channel& 	operator=(const Channel& src);
	//joinClient
	void joinClient(std::string* userId, const std::string password, const bool& isAdmin);
	void joinClient(vectorStr& users, std::string* userId, const bool& isAdmin);
	//deleteClient
	void deleteClient(const std::string* userId);
	void deleteClient(vectorStr& users, const std::string* userId);
	//kick
	//invite
	void checkInvite(const std::string* senderId);
	//changeTopic
	void changeTopic(const std::string* userId, const std::string value);
	//changeInviteMode
	void changeInviteMode(const std::string* userId, const bool& value);
	//changePassword
	vectorStr::iterator findUser(vectorStr& users, const std::string* userId) ;
	//mode set/remove channel key newPass "" para remove y con valor para set
	void changePassword(const std::string* userId, const std::string newPassword);
	//mode give/take channnel operator privilege
	void giveOperatorPrivilege(const std::string* senderId, const std::string* srcUser);
	void takeOperatorPrivilege(const std::string* senderId, const std::string* srcUser);
	//Check if the user is in the channel
	bool isRegister(std::string* userId);

	vectorStr	getUsers() const;
	vectorStr	getAdmins() const;
	str		getTopic() const;
	str		getId() const;
	str		getPassword() const;
	int 		getMaxUsers() const;
	bool 		getInviteMode() const;
	bool		getTopicRestriction() const;

	void		setUsers(vectorStr src);
	void		setAdmins(vectorStr src);
	void		setTopic(str src);
	void		setId(str src);
	void		setPassword(str src);
	void		setMaxUsers(int src);
	void		setInviteMode(bool src);
	void		setTopicRestriction(const bool& src);
	void		printAdminUsers() const ;
	
private:
	//vectorStr	users;
	//cambio prov a &str
	vectorStr	users;
	std::vector<std::string *>	admins;
	std::string 	topic;
	std::string 	id;
	std::string 	password;
	unsigned int 	maxUsers;
	bool		inviteMode;
	bool		topicRestriction;
};

#endif     //CHANNEL_H
