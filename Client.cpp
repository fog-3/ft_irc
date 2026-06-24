#include "Client.hpp"

Client::Client() : _fd(0), _nickname(), _username(), _authenticated(false), _registered(false), _buffer()
{
	std::cout << "Client Default constructor has been called." << std::endl;
}

Client::Client(Client const &copy) : _fd(copy._fd), _nickname(copy._nickname), _username(copy._username), _authenticated(copy._authenticated), _registered(copy._registered), _buffer(copy._buffer)
{
	std::cout << "Client Copy constructor has been called." << std::endl;
}

Client &Client::operator= (Client const &copy)
{
	std::cout << "Client assingment operator has been called." << std::endl;
	if (this != &copy)
	{
		_fd = copy._fd;
		_nickname = copy._nickname;
		_username = copy._username;
		_authenticated = copy._authenticated;
		_registered = copy._registered;
		_buffer = copy._buffer;
	}
	return (*this);
}

Client::~Client()
{
	std::cout << "Client destructor has been called." << std::endl;
}

bool	Client::getAuthenticated() const
{
	return (this->_authenticated);
}

void	Client::setAuthenticated(bool set)
{
	this->_authenticated = set;
}
