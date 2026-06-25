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

	bool		getAuthenticated() const;
	void		setAuthenticated(bool set);
	bool		getRegistered() const;
	void		setRegistered(bool set);
	std::string	getNickname() const;
	void		setNickname(std::string nick);
	std::string	getUsername() const;
	void		setUsername(std::string user);
	
	private:
	int			_fd;
	std::string	_nickname;
	std::string	_username;
	bool		_authenticated;
	bool		_registered;
	std::string	_buffer;
};

#endif
