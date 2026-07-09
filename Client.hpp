#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <cstdlib>

class Client
{
	public:
		Client();
		Client(int fd, const std::string& ip);
		Client(Client const &copy);
		Client &operator=(Client const &copy);
		~Client();
		
		int			getFd() const;
		void		setFd(int fd);

		bool		getAuthenticated() const;
		void		setAuthenticated(bool set);

		bool		getRegistered() const;
		void		setRegistered(bool set);

		std::string	getNickname() const;
		void		setNickname(std::string nick);

		std::string	getUsername() const;
		void		setUsername(std::string user);

		const		std::string& getIp() const;
		const		std::string& getBuffer() const;

		const 		std::string& getOutputBuffer() const;

		
		void		appendBuffer(const std::string& buffer);
		void		eraseBuffer(int length);

		void		appendOutputBuffer(const std::string& buffer);
		void		eraseOutputBuffer(int length);
	
	private:
		int			_fd;
		std::string	_ip;
		std::string	_nickname;
		std::string	_username;
		bool		_authenticated;
		bool		_registered;
		std::string	_buffer;
		std::string _outputBuffer;
};

#endif
