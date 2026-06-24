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

void	Server::run()
{}
