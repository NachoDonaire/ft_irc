#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	protected:
		std::string	msg;
		std::vector<std::string>	params;
		std::string	psswd;
		std::string	servPsswd;
		int	 	fd;
 	public:
		Client(int fd, std::string psswdServ);
		void	setMsg(std::string message);
		int	parseMsg();
		std::string getMsg() { return this->msg;};
    		Client();
    		~Client();
		void	cmdAnalyzer();
};

#endif
