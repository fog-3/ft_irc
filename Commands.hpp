#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <algorithm>
#include "Server.hpp"

void	cmdPass(Server &serv, Client &client, Message &msg);
void	cmdNick(Server &serv, Client &client, Message &msg);
void	cmdUser(Server &serv, Client &client, Message &msg);
void	cmdJoin(Server &serv, Client &client, Message &msg);

#endif