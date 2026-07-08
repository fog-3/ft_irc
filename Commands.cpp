#include "Commands.hpp"

void	cmdPass(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		sendError(client, 461);
		return ;
	}
	if (msg.params[0] == serv.getPassword())
		client.setAuthenticated(true);
	else
	{
		sendError(client, 464);
		return ;
	}
}

void	cmdNick(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		sendError(client, 431);
		return ;
	}
	if (!client.getAuthenticated())
	{
		sendError(client, 451);
		return ;
	}
	if (serv.isNickTaken(msg.params[0]))
	{
		sendError(client, 433);
		return ;
	}
	client.setNickname(msg.params[0]);
	if (client.getUsername() != "")
	{
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() + " :Welcome to the IRC server "
			+ client.getNickname() + "!" + client.getUsername() + "@ircserv\r\n");
	}
}

void	cmdUser(Server &serv, Client &client, Message &msg)
{
	(void)serv;
	if (client.getUsername() != "")
	{
		sendError(client, 462);
		return ;
	}
	if (msg.params.empty())
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getAuthenticated())
	{
		sendError(client, 451);
		return ;
	}
	client.setUsername(msg.params[0]);
	if (client.getNickname() != "")
	{
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() + " :Welcome to the IRC server "
			+ client.getNickname() + "!" + client.getUsername() + "@ircserv\r\n");
	}
}

void	cmdJoin(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel *chan = serv.findChannel(msg.params[0]);
	if (chan)
	{
		std::map<Client*, bool> members = chan->getMembers();
		if (chan->getLimit() != -1 && static_cast<size_t>(chan->getLimit()) < members.size() + 1)
		{
			sendError(client, 471);
			return ;
		}
		if (members.find(&client) != members.end())
		{
			sendError(client, 443);
			return ;
		}
		if (chan->getInviteOnly())
		{
			std::vector<std::string>	inviteList = chan->getInviteList();
			std::vector<std::string>::iterator it = std::find(inviteList.begin(), inviteList.end(), client.getNickname());
			if (it == inviteList.end())
			{
				sendError(client, 473);
				return ;
			}
		}
		if (chan->getKey() != "" && (msg.params.size() < 2 || msg.params[1] != chan->getKey()))
		{
			sendError(client, 475);
			return ;
		}
		chan->addMember(&client);
		for (std::map<Client*, bool>::iterator it2 = members.begin(); it2 != members.end(); ++it2)
			sendToClient(*it2->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv JOIN "
				+ chan->getName() + "\r\n");
		sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv JOIN "
			+ chan->getName() + "\r\n");
	}
	else
	{
		Channel *newChan = new Channel(msg.params[0]);
		serv.addChannel(newChan);
		newChan->addMember(&client);
		sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv JOIN "
			+ newChan->getName() + "\r\n");
	}
}

void	cmdPrivmsg(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (msg.params[0][0] == '#' && chan)
	{
		std::map<Client*, bool> members = chan->getMembers();
		for (std::map<Client*, bool>::iterator it = members.begin(); it != members.end(); ++it)
		{
			sendToClient(*it->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv PRIVMSG "
				+ msg.params[0] + " :" + msg.params[1] + "\r\n");
		}
	}
	else
	{
		Client	*addr = serv.findClient(msg.params[0]);
		if (!addr)
		{
			sendError(client, 401);
			return ;
		}
		sendToClient(*addr, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv PRIVMSG "
			+ msg.params[0] + " :" + msg.params[1] + "\r\n");
	}
}

void	cmdPart(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		sendError(client, 403);
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	if (members.find(&client) == members.end())
	{
		sendError(client, 442);
		return ;
	}
	for (std::map<Client*, bool>::iterator it = members.begin(); it != members.end(); ++it)
	{
		sendToClient(*it->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv PART "
			+ chan->getName() + "\r\n");
	}
	chan->removeMember(&client);
}

void	cmdKick(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		sendError(client, 403);
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	std::map<Client *, bool>::iterator it = members.find(&client);
	if (it == members.end())
	{
		sendError(client, 442);
		return ;
	}
	if (!it->second)
	{
		sendError(client, 482);
		return ;
	}
	Client	*rem = serv.findClient(msg.params[1]);
	if (!rem)
	{
		sendError(client, 401);
		return ;
	}
	std::string	reason = msg.params.size() >= 3 ? msg.params[2] : msg.params[1];
	for (std::map<Client*, bool>::iterator it = members.begin(); it != members.end(); ++it)
	{
		sendToClient(*it->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv KICK "
			+ chan->getName() + " " + msg.params[1] + " :" + reason + "\r\n");
	}
	if (members.find(rem) != members.end())
		chan->removeMember(rem);
	else
	{
		sendError(client, 442);
		return ;
	}
}

void	cmdInvite(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[1]);
	if (!chan)
	{
		sendError(client, 403);
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	std::map<Client *, bool>::iterator it = members.find(&client);
	if (it == members.end())
	{
		sendError(client, 442);
		return ;
	}
	if (!it->second)
	{
		sendError(client, 482);
		return ;
	}
	Client	*inv = serv.findClient(msg.params[0]);
	if (!inv)
	{
		sendError(client, 401);
		return ;
	}
	chan->addToInviteList(inv->getNickname());
	sendToClient(*inv, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv INVITE "
		+ inv->getNickname() + " :" + chan->getName() + "\r\n");
}

void	cmdTopic(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 1)
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		sendError(client, 403);
		return ;
	}
	if (msg.params.size() < 2)
	{
		std::string	topic = chan->getTopic();
		if (topic.empty())
		{
			sendError(client, 331);
			return ;
		}
		sendToClient(client, ":ircserv 332 " + client.getNickname() + " " + chan->getName() + " :" + topic + "\r\n");
	}
	else
	{
		std::map<Client*, bool> members = chan->getMembers();
		std::map<Client *, bool>::iterator it = members.find(&client);
		if (it == members.end())
		{
			sendError(client, 442);
			return ;
		}
		if (!chan->getTopicRestricted() || it->second)
		{
			chan->setTopic(msg.params[1]);
		}
		else
		{
			sendError(client, 482);
			return ;
		}
	}
}

void	cmdMode(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		sendError(client, 461);
		return ;
	}
	if (!client.getRegistered())
	{
		sendError(client, 451);
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		sendError(client, 403);
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	std::map<Client *, bool>::iterator it = members.find(&client);
	if (it == members.end())
	{
		sendError(client, 442);
		return ;
	}
	if (!it->second)
	{
		sendError(client, 482);
		return ;
	}
	if (msg.params[1] == "+i")
		chan->setInviteOnly(true);
	else if (msg.params[1] == "-i")
		chan->setInviteOnly(false);
	else if (msg.params[1] == "+t")
		chan->setTopicRestricted(true);
	else if (msg.params[1] == "-t")
		chan->setTopicRestricted(false);
	else if (msg.params[1] == "+k" && msg.params.size() == 3)
		chan->setKey(msg.params[2]);
	else if (msg.params[1] == "-k")
		chan->setKey("");
	else if (msg.params[1] == "+l" && msg.params.size() == 3)
		chan->setLimit(atoi(msg.params[2].c_str()));
	else if (msg.params[1] == "-l")
		chan->setLimit(-1);
	else if (msg.params[1] == "+o" && msg.params.size() == 3)
	{
		Client	*rem = serv.findClient(msg.params[2]);
		if (!rem)
		{
			sendError(client, 401);
			return ;
		}
		std::map<Client *, bool>::iterator it2 = members.find(rem);
		if (it2 == members.end())
		{
			sendError(client, 442);
			return ;
		}
		chan->setOperator(rem, true);
	}
	else if (msg.params[1] == "-o" && msg.params.size() == 3)
	{
		Client	*rem2 = serv.findClient(msg.params[2]);
		if (!rem2)
		{
			sendError(client, 401);
			return ;
		}
		std::map<Client *, bool>::iterator it3 = members.find(rem2);
		if (it3 == members.end())
		{
			sendError(client, 442);
			return ;
		}
		chan->setOperator(rem2, false);
	}
}

void	cmdPing(Server &serv, Client &client, Message &msg)
{
	(void)serv;
	if (msg.params.empty())
	{
		sendError(client, 461);
		return ;
	}
	sendToClient(client, ":ircserv PONG ircserv :" + msg.params[0] + "\r\n");
}

void	cmdQuit(Server &serv, Client &client, Message &msg)
{
	(void)serv;
	(void)client;
	(void)msg;
	// TODO
}
