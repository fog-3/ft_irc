#include "Client.hpp"

Client::Client() : _fd(0), _ip(), _nickname(), _username(), _authenticated(false), _registered(false), _buffer()
{
	std::cout << "Client Default constructor has been called." << std::endl;
}

Client::Client(int fd, const std::string& ip) : _fd(fd), _ip(ip),  _nickname(), _username(), _authenticated(false), _registered(false), _buffer()
{
	std::cout << "Client with the ip " + ip + " and the fd " << fd << " created" << std::endl;
}

Client::Client(Client const &copy) : _fd(copy._fd), _ip(copy._ip), _nickname(copy._nickname), _username(copy._username), _authenticated(copy._authenticated), _registered(copy._registered), _buffer(copy._buffer)
{
	std::cout << "Client Copy constructor has been called." << std::endl;
}

Client &Client::operator= (Client const &copy)
{
	std::cout << "Client assingment operator has been called." << std::endl;
	if (this != &copy)
	{
		_fd = copy._fd;
		_ip = copy._ip;
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

int	Client::getFd() const{
	return _fd;
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

const std::string& Client::getIp() const{
	return _ip;
}

const std::string& Client::getBuffer() const{
	return _buffer;
}

bool	Client::getAuthenticated() const
{
	return (this->_authenticated);
}

void	Client::setAuthenticated(bool set)
{
	this->_authenticated = set;
}

bool	Client::getRegistered() const
{
	return (this->_registered);
}

void	Client::setRegistered(bool set)
{
	this->_registered = set;
}

std::string	Client::getNickname() const
{
	return (this->_nickname);
}

void	Client::setNickname(std::string nick)
{
	this->_nickname = nick;
}

std::string	Client::getUsername() const
{
	return (this->_username);
}

void	Client::setUsername(std::string user)
{
	this->_username = user;
}

void Client::appendBuffer(const std::string& buffer){
	_buffer += buffer;
}

void Client::eraseBuffer(int length){
	_buffer.erase(0, length);
}
