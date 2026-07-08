#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <cstdlib>
#include <vector>

class Bot
{
	public:
	Bot();
	Bot(const Bot &copy);
	Bot	&operator=(const Bot &copy);
	~Bot();

	private:
	int							_fd;
	std::string					_nick;
	std::vector<std::string>	_channelS;
	std::string					_buffer;
};

void	registerBot();
void	connectToServer();

#endif
