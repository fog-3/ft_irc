#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include "Message.hpp"
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
	void run();
	
	private:
	int _port;
	std::string _password;
	std::map<int, Client*> _clients;
	std::map<std::string, Channel*> _channels;
	std::vector<pollfd> _pollfds;
};

#endif