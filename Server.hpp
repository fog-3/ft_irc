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

	void run();
	
	private:
	int _port;
	int _serverFd;
	std::string _password;
	std::map<int, Client*> _clients;
	std::map<std::string, Channel*> _channels;
	std::vector<struct pollfd> _pollfds;

	// --- Net helpers ---
	void initServer();				// socket(), bind(), listen()
	void acceptNewClient();			// accept()
	void readFromClient(int fd);	// recv()
	void disconnectClient(int fd);	// close() y limpieza de memoria

	// --- El puente lógico ---
	void precessLine(Client& sender, const std::string& line);
};

#endif