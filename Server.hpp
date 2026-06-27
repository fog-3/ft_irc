#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	public:
	Server();
	Server(int port, std::string password);
	Server(Server const &copy);
	Server &operator=(Server const &copy);
	~Server();

	std::string	getPassword() const;
	Channel*	findChannel(std::string name);
	void		addChannel(Channel *channel);
	bool		isNickTaken(std::string nick);
	Client*		findClient(std::string nick);
	void		run();
	
	private:
	int 							_port;
	std::string 					_password;
	std::map<int, Client*> 			_clients;
	std::map<std::string, Channel*> _channels;
	std::vector<pollfd> 			_pollfds;
};

#endif
