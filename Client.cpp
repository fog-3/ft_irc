#include "Client.hpp"

Client::Client() : _fd(0), _ip(), _nickname(), _username(), _authenticated(false), _registered(false), _buffer()
{
	std::cout << "Client Default constructor has been called." << std::endl;
}

Client::Client(int fd, const std::string& ip) : _fd(fd), _ip(ip),  _nickname(), _username(), _authenticated(false), _registered(false), _buffer()
{
	std::cout << "Client with the ip " + ip + " and the fd " << fd << " created" << std::endl;
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

int	Client::getFd() const{
	return _fd;
}

const std::string& Client::getIp() const{
	return _ip;
}

const std::string& Client::getBuffer() const{
	return _buffer;
}

void Client::appendBuffer(const std::string& buffer){
	_buffer += buffer;
}

void Client::eraseBuffer(int length){
	_buffer.erase(0, length);
}
