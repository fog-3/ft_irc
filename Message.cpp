#include "Message.hpp"
#include "Commands.hpp"

void	processLine(Server &serv, Client &client, std::string msg)
{
	Message mes = Parser(msg);
	if (mes.command == "PASS")
		cmdPass(serv, client, mes);
	else if (mes.command == "NICK")
		cmdNick(serv,client, mes);
	else if (mes.command == "USER")
		cmdUser(serv,client, mes);
	else if (mes.command == "JOIN")
		cmdJoin(serv,client, mes);
	else if (mes.command == "PRIVMSG")
		cmdPrivmsg(serv,client, mes);
	else if (mes.command == "PART")
		cmdPart(serv,client, mes);
	else if (mes.command == "KICK")
		cmdKick(serv,client, mes);
	else if (mes.command == "INVITE")
		cmdInvite(serv,client, mes);
	else if (mes.command == "TOPIC")
		cmdTopic(serv,client, mes);
	else if (mes.command == "MODE")
		cmdMode(serv,client, mes);
	else if (mes.command == "PING")
		cmdPing(serv, client, mes);
	else if (mes.command == "QUIT")
		cmdQuit(serv, client, mes);
	else if (mes.command == "QUIT_SERVER")
		cmdQuitServer(serv, client, mes);
	else
	{
		sendError(client, 421, mes.command);
		return ;
	}
}

void	sendToClient(Client &client, std::string msg)
{
	client.appendOutputBuffer(msg);
}

void	sendError(Client &client, int code, std::string context)
{
	std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
	if (code == 431)
		sendToClient(client, ":ircserv 431 " + nick + " :No nickname given\r\n");
	else if (code == 432)
		sendToClient(client, ":ircserv 432 " + nick + " :Erroneous nickname\r\n");
	else if (code == 433)
		sendToClient(client, ":ircserv 433 " + nick + " " + context + " :Nickname already in use\r\n");
	else if (code == 451)
		sendToClient(client, ":ircserv 451 " + nick + " :Not registered\r\n");
	else if (code == 461)
		sendToClient(client, ":ircserv 461 " + nick + " :Not enough parameters\r\n");
	else if (code == 462)
		sendToClient(client, ":ircserv 462 " + nick + " :Already registered\r\n");
	else if (code == 403)
		sendToClient(client, ":ircserv 403 " + nick + " :No such channel\r\n");
	else if (code == 404)
		sendToClient(client, ":ircserv 404 " + nick + " :Cannot send to channel\r\n");
	else if (code == 401)
		sendToClient(client, ":ircserv 401 " + nick + " " + context + " :No such nickname\r\n");
	else if (code == 442)
		sendToClient(client, ":ircserv 442 " + nick + " :Not on channel\r\n");
	else if (code == 482)
		sendToClient(client, ":ircserv 482 " + nick + " :Not channel operator\r\n");
	else if (code == 471)
		sendToClient(client, ":ircserv 471 " + nick + " " + context + " :Channel is full(limit)\r\n");
	else if (code == 473)
		sendToClient(client, ":ircserv 473 " + nick + " " + context + " :Invite only channel\r\n");
	else if (code == 475)
		sendToClient(client, ":ircserv 475 " + nick + " " + context + " :Bad channel key\r\n");
	else if (code == 464)
		sendToClient(client, ":ircserv 464 " + nick + " :Password incorrect\r\n");
	else if (code == 443)
		sendToClient(client, ":ircserv 443 " + nick + " :Is already on channel\r\n");
	else if (code == 331)
		sendToClient(client, ":ircserv 331 " + nick + " :No topic is set\r\n");
	else if (code == 421)
		sendToClient(client, ":ircserv 421 " + nick + " " + context + " :Unknown command\r\n");
	else if (code == 472)
		sendToClient(client, ":ircserv 472 " + nick + " " + context + " :Is unknown mode char to me\r\n");
}
