#ifndef CHANNEL_H
#define CHANNEL_H
#include <iostream>
#include <string>
typedef std::vector<std::string> 	vectorStr;
typedef std::string 			str;

class Channel
{
public:
	Channel();
	~Channel();
   	Channel(const Channel& src);
	Channel(const str& id);
   	Channel& 	operator=(const Channel& src);
	//joinClient
	void joinClient(const str& userId, const bool& isAdmin);
	void joinClient(vectorStr& users, const str& userId, const bool& isAdmin);
	//deleteClient
	void deleteClient(const str& userId);
	void deleteClient(vectorStr& users, const str& userId);
	//kick
	//invite
	void checkInvite(const str& senderId);
	//changeTopic
	void changeTopic(const str& userId, const str& value);
	//changeInviteMode
	void changeInviteMode(const str& userId, const bool& value);
	//changePassword
	vectorStr::iterator findUser(vectorStr& users, const str& userId) const;
	//mode set/remove channel key newPass "" para remove y con valor para set
	void changePassword(const str& userId, const str& newPassword);
	//mode give/take channnel operator privilege
	void giveOperatorPrivilege(const str& senderId, const str& srcUser);
	void takeOperatorPrivilege(const str& senderId, const str& srcUser);

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
	
private:
	vectorStr	users;
	vectorStr 	admins;
	std::string 	topic;
	std::string 	id;
	std::string 	password;
	int 		maxUsers;
	bool		inviteMode;
	bool		topicRestriction;
};

#endif     //CHANNEL_H
