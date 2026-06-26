#include "Server.hpp"

Server::Server() : _port(0), _password(), _clients(), _channels(), _pollfds()
{
	std::cout << "Server Default constructor has been called." << std::endl;
}

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	std::cout << "Server Parameterized constructor has been called." << std::endl;
}

Server::Server(Server const &copy) : _port(copy._port), _password(copy._password), _clients(copy._clients), _channels(copy._channels), _pollfds(copy._pollfds)
{
	std::cout << "Server Copy constructor has been called." << std::endl;
}

Server &Server::operator= (Server const &copy)
{
	std::cout << "Server assingment operator has been called." << std::endl;
	if (this != &copy)
	{
		_port = copy._port;
		_password = copy._password;
		_clients = copy._clients;
		_channels = copy._channels;
		_pollfds = copy._pollfds;
	}
	return (*this);
}

Server::~Server()
{
	std::cout << "Server destructor has been called." << std::endl;
}

std::string	Server::getPassword() const
{
	return (this->_password);
}

Channel* Server::findChannel(std::string name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return (NULL);
	return (it->second);
}

void	Server::addChannel(Channel *channel)
{
	_channels[channel->getName()] = channel;
}

bool	Server::isNickTaken(std::string nick)
{
	for	(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
			return (true);
	}
	return (false);
}

Client*	Server::findClient(std::string nick)
{
	for	(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
			return (it->second);
	}
	return (NULL);
}

void	Server::run()
{}
