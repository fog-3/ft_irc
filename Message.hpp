#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>

struct Message
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	params;
	std::string					trailing;
};

Message	Parser(std::string mes);

#endif
