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
		
		int getFd() const;
		const std::string& getIp() const;
		const std::string& getBuffer() const;
		
		void appendBuffer(const std::string& buffer);
		void eraseBuffer(int length);

	private:
		int			_fd;
		std::string	_ip;
		std::string	_nickname;
		std::string	_username;
		bool		_authenticated;
		bool		_registered;
		std::string	_buffer;
};

#endif