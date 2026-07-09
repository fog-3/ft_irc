#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include "Commands.hpp"

Server::Server() : _port(0), _password(), _clients(), _channels(), _pollfds()
{
	std::cout << "[SERVER] Default constructor has been called." << std::endl;
}

Server::Server(int port, std::string password) : _port(port), _serverFd(-1), _password(password), _clients(), _channels(), _pollfds()
{
	initServer();
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
	if (_serverFd != -1){
		close(_serverFd);
	}

	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		delete it->second;
	}

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		delete it->second;
	}

	std::cout << "Server destructor has been called." << std::endl;
}

// --- Net initialization
void	Server::initServer(){
	/* Socket creation:
	* AF_INET: Address family -> you are telling the OS that you want to use IPv4 addresses
	* SOCK_STREAM: is doing the TCP connection which guarantees that the information arrives
	* in the correct order, without missing letters
	*/
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1){
		throw std::runtime_error("Error: Could not create socket");
	}

	/* Allow to reuse the port if the server crash
	*  SO_REUSEADDR: allows the socket to be bound to an address that is already in use
	*  SOL_SOCKET: specifies that the option is being set at the socket level
	*/
	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
		throw std::runtime_error("Error: setsocket failed");
	}

	/* Socket mode "NOT BLOCK"
	*  F_SETFL: file descriptor modification
	*  O_NONBLOCK: if there is a problem reading the data return an error (-1)
	*/
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1){
		throw std::runtime_error("Error: fcntl failed");
	}

	// Port and direction configuration
	struct sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // That makes the server listen for incoming connection on all network interfaces.
	serverAddr.sin_port = htons(_port); // Host flips the bytes safely so network stack can actually read it.

	/* Link the socket to the port (bind) */
	if (bind(_serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
		throw std::runtime_error("Error: bind failed. Is the port already in use?");
	}

	// Starts listening
	if (listen(_serverFd, SOMAXCONN) == -1){
		throw std::runtime_error("Error: listen failed");
	}

	std::cout << "Server successfully initialized and listening on port " << _port << std::endl;
}

void	Server::run(){
	std::cout << "Waiting in the void.." << std::endl;

	struct pollfd main_poll;
	main_poll.fd = _serverFd;
	main_poll.events = POLLIN;
	main_poll.revents = 0;
	_pollfds.push_back(main_poll);

	while (true) {
		// The timeout is -1 to wait infinitely until a new message come
		poll(&_pollfds[0], _pollfds.size(), -1);		
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents & POLLIN)
			{
				if (_pollfds[i].fd == _serverFd)
					acceptNewClient();
				else
					readFromClient(_pollfds[i].fd);
			}
		}
	}
}

void	Server::acceptNewClient() {
	// Port and direction configuration
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int newClientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &clientLen); 

	// Avoiding the block 
	if (newClientFd == -1) {
		std::cerr << "[SERVER] Error accepting new client." << std::endl;
		return; // Don't crash the server, just ignore the failed connection
    }
	
	if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) == -1){
		std::cerr << "[SERVER] Error setting client to non-blocking." << std::endl;
        close(newClientFd);
		return;
	}

	struct pollfd newPollfd;
	newPollfd.fd = newClientFd;
	newPollfd.events = POLLIN;
	newPollfd.revents = 0;
	_pollfds.push_back(newPollfd);

	std::string clientIP = inet_ntoa(clientAddr.sin_addr);

	_clients[newClientFd] = new Client(newClientFd, clientIP);
	std::cout << "[SERVER] New connection from " << clientIP << " assigned FD " << newClientFd << std::endl;
}

void	Server::readFromClient(int client_fd){
	char buffer[1024];

	int bytesRead = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytesRead > 0) {
		Client* currentClient = _clients[client_fd];
		buffer[bytesRead] = '\0';
		currentClient->appendBuffer(buffer);

		std::string bufferString = currentClient->getBuffer();
		const long unsigned int endMessage = bufferString.find("\r\n");
		const long unsigned int endMessage2 = bufferString.find("\n");
		if (endMessage != std::string::npos){
			std::string command = bufferString.substr(0, endMessage);
			processLine(*this, *currentClient, command);
			currentClient->eraseBuffer(endMessage + 2);
		}
		else if (endMessage2 != std::string::npos)
		{
			std::string command = bufferString.substr(0, endMessage2);
			processLine(*this, *currentClient, command);
			currentClient->eraseBuffer(endMessage2 + 1);
		}
	} else if (bytesRead == 0) {
		disconnectClient(client_fd);
		std::cout << "[SERVER] The client " << client_fd << " has disconnected" << std::endl;
	} else {
		std::cerr << "[SERVER] Error reading the socket message" << std::endl;
	}
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

void	Server::removeChannel(std::string channel)
{
	delete	_channels[channel];
	_channels.erase(channel);
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

std::map<std::string, Channel*>	Server::getChannels() const
{
	return (this->_channels);
}

void	Server::disconnectClient(int fd)
{
	close(fd);
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == fd)
			_pollfds.erase(_pollfds.begin() + i);
	}
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); )
	{
		it->second->removeMember(_clients[fd]);
		if (it->second->getMembers().empty())
		{
			delete it->second;
			it = _channels.erase(it);
		}
		else
			++it;
	}
	delete _clients[fd];
	_clients.erase(fd);
}
