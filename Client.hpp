#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <cstdlib>

class Client
{
	public:
	Client();
	Client(Client const &copy);
	Client &operator=(Client const &copy);
	~Client();
	
	private:
	int			_fd;
	std::string	_nickname;
	std::string	_username;
	bool		_authenticated;
	bool		_registered;
	std::string	_buffer;
};

#endif