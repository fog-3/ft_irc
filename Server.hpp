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
		void		setLooping(bool isLooping);
		Channel*	findChannel(std::string name);
		void		addChannel(Channel *channel);
		void		removeChannel(std::string channel);
		bool		isNickTaken(std::string nick);
		Client*		findClient(std::string nick);
		void		run();
		void		disconnectClient(int fd);	// close() and free memory
		std::map<std::string, Channel*>	getChannels() const;
	
	private:
		int _port;
		int _serverFd;
		std::string _password;
		std::map<int, Client*> _clients;
		std::map<std::string, Channel*> _channels;
		std::vector<struct pollfd> _pollfds;
		bool _isLooping;

		// --- Net helpers ---
		void	initServer();				// socket(), bind(), listen()
		void	acceptNewClient();			// accept()
		bool	readFromClient(int fd);	// recv()
		void	writeToClient(int fd);	// send()

		// --- Other helpers ---
		void printBanner() const;
};

#endif
