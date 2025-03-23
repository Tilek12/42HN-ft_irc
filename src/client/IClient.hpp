
#ifndef CLIENT_HPP
#define CLINET_HPP
#include <iostream>

class IClient {

		public:

			/*Getters for nickname, username, realname and hostname*/
			virtual int	getSockedFd() const = 0;
			virtual std::string getNickname() const = 0;
			virtual std::string getUsername() const = 0;
			virtual std::string getRealname() const = 0;
			virtual std::string getHostname() const = 0;
			virtual bool		getIsResgistered() const = 0;

			/*Setters*/
			virtual void	setSockedFd(int fd) = 0;
			virtual void	setNickname(std::string &nickname) = 0;
			virtual void	setUsername(std::string &username) = 0;
			virtual void	setRealname(std::string &realname) = 0;
			virtual void	setHostname(std::string &hostname) = 0;
			virtual void	setIsRegistered(bool status) = 0;
			
			/*Destructor*/
			virtual ~IClient();

};


#endif
