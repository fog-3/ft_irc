#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <algorithm>
#include <sys/socket.h>
#include "Message.hpp"
#include "Server.hpp"

void	sendError(Client &client, int code, std::string context = "");
void	sendToClient(Client &client, std::string msg);
void	processLine(Server &serv, Client &client, std::string msg);

void	cmdPass(Server &serv, Client &client, Message &msg);
void	cmdNick(Server &serv, Client &client, Message &msg);
void	cmdUser(Server &serv, Client &client, Message &msg);
void	cmdJoin(Server &serv, Client &client, Message &msg);
void	cmdPrivmsg(Server &serv, Client &client, Message &msg);
void	cmdPart(Server &serv, Client &client, Message &msg);
void	cmdKick(Server &serv, Client &client, Message &msg);
void	cmdInvite(Server &serv, Client &client, Message &msg);
void	cmdTopic(Server &serv, Client &client, Message &msg);
void	cmdMode(Server &serv, Client &client, Message &msg);
void	cmdPing(Server &serv, Client &client, Message &msg);
void	cmdQuit(Server &serv, Client &client, Message &msg);
void	cmdQuitServer(Server &serv, Client &client, Message &msg);

#endif
