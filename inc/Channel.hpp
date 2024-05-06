#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <Client.hpp>

class Channel
{
	private:
		std::vector<Client> clients;
		std::vector<Client> operators;
		std::string	topic;
 	public:
    		Channel();
    		Channel(Client op);
    		~Channel();
};

#endif
