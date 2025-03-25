
#ifndef ICLIENT_HPP
#define ICLIENT_HPP
#include <iostream>

class IClient {

		public:

			/*Getters for nickname, username, realname and hostname*/
			virtual int	getSocketFd() const = 0;
			virtual std::string getNickname() const = 0;
			virtual std::string getUsername() const = 0;
			virtual std::string getRealname() const = 0;
			virtual std::string getHostname() const = 0;
			virtual bool		getIsResgistered() const = 0;

			/*Setters*/
			virtual void	setSocketFd(int fd) = 0;
			virtual void	setNickname(std::string &nickname) = 0;
			virtual void	setUsername(std::string &username) = 0;
			virtual void	setRealname(std::string &realname) = 0;
			virtual void	setHostname(std::string &hostname) = 0;
			virtual void	setIsRegistered(bool status) = 0;

			/*Destructor*/
			virtual ~IClient() {}

			// Buffer management
			virtual void			appendToBuffer(const char* data, size_t len) = 0;
			virtual void			clearBuffer() = 0;
			virtual std::string&	getBuffer() = 0;
			virtual bool			hasPendingData() const = 0;
			virtual void			queueMessage(const std::string& message) = 0;
			virtual std::string		getNextMessage() = 0;

};


#endif
