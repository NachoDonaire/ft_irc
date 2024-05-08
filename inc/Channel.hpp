#ifndef CHANNEL_H
#define CHANNEL_H
#include <iostream>
#include <string>

class Channel
{
public:
	Channel();
	~Channel();
   Channel(const Channel& src);
   Channel& operator=(const Channel& src);
	//joinClient
	//deleteClient
	//kick
	//invite
	//changeTopic
	//changeInviteMode
	//changePassword
private:
	//vector de usuarios del canal
	//vector de admin users
	//std::string topic
	//std::string idName
	//std::string password
	//vector de idUsers blacklist
	//vector de idUsers whitelist
	//bool invite
	//int maxUsers;
};

#endif     //CHANNEL_H
