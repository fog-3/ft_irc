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
	std::string	oldNick = client.getNickname();
	bool	wasRegistered = client.getRegistered();
	client.setNickname(msg.params[0]);
	if (client.getUsername() != "" && !client.getRegistered())
	{
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() + " :Welcome to the IRC server "
			+ client.getNickname() + "!" + client.getUsername() + "@ircserv\r\n");
	}
	if (wasRegistered)
	{
		std::map<std::string, Channel*>	chan = serv.getChannels();
		for (std::map<std::string, Channel*>::iterator it = chan.begin(); it != chan.end(); ++it)
		{
			std::map<Client *, bool>	members = it->second->getMembers();
			std::map<Client *, bool>::iterator it2 = members.find(&client);
			if (it2 != members.end())
			{
				for (std::map<Client*, bool>::iterator it3 = members.begin(); it3 != members.end(); ++it3)
					sendToClient(*it3->first, ":" + oldNick + "!" + client.getUsername() + "@ircserv NICK :"
						+ client.getNickname() + "\r\n");
			}
		}
		sendToClient(client, ":" + oldNick + "!" + client.getUsername() + "@ircserv NICK :" + client.getNickname() + "\r\n");
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
		chan = newChan;
	}
	std::string nicks = "";
	std::map<Client*, bool> members2 = chan->getMembers();
	for (std::map<Client*, bool>::iterator it3 = members2.begin(); it3 != members2.end(); ++it3)
	{
		nicks += it3->first->getNickname();
		nicks += " ";
	}
	sendToClient(client, ":ircserv 353 " + client.getNickname() + " = " + chan->getName() + " :" + nicks + "\r\n");
	sendToClient(client, ":ircserv 366 " + client.getNickname() + " " + chan->getName() + " :End of /NAMES list\r\n");
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
		if (members.find(&client) == members.end())
		{
			sendError(client, 442);
			return ;
		}
		for (std::map<Client*, bool>::iterator it = members.begin(); it != members.end(); ++it)
		{
			if (it->first == &client)
				continue ;
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
	if (chan->getMembers().empty())
		serv.removeChannel(chan->getName());
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
	if (members.find(rem) != members.end())
		chan->removeMember(rem);
	else
	{
		sendError(client, 442);
		return ;
	}
	std::string	reason = msg.params.size() >= 3 ? msg.params[2] : "No reason";
	for (std::map<Client*, bool>::iterator it = members.begin(); it != members.end(); ++it)
	{
		sendToClient(*it->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv KICK "
			+ chan->getName() + " " + msg.params[1] + " :" + reason + "\r\n");
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
			for (std::map<Client*, bool>::iterator it2 = members.begin(); it2 != members.end(); ++it2)
				sendToClient(*it2->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv TOPIC "
					+ chan->getName() + " :" + chan->getTopic() + "\r\n");
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
	else if (msg.params[1] == "+k" && msg.params.size() >= 3)
		chan->setKey(msg.params[2]);
	else if (msg.params[1] == "-k")
		chan->setKey("");
	else if (msg.params[1] == "+l" && msg.params.size() >= 3)
		chan->setLimit(atoi(msg.params[2].c_str()));
	else if (msg.params[1] == "-l")
		chan->setLimit(-1);
	else if (msg.params[1] == "+o" && msg.params.size() >= 3)
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
	else if (msg.params[1] == "-o" && msg.params.size() >= 3)
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
	std::string	reason = msg.params.size() >= 3 ? " " + msg.params[2] : "";
	for (std::map<Client*, bool>::iterator it2 = members.begin(); it2 != members.end(); ++it2)
	{
		sendToClient(*it2->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv MODE "
			+ chan->getName() + " " + msg.params[1] + reason + "\r\n");
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
	std::map<std::string, Channel*>	chan = serv.getChannels();
	for (std::map<std::string, Channel*>::iterator it = chan.begin(); it != chan.end(); ++it)
	{
		std::map<Client *, bool>	members = it->second->getMembers();
		std::map<Client *, bool>::iterator it2 = members.find(&client);
		if (it2 != members.end())
		{
			std::string reason = msg.params.empty() ? "" : " :" + msg.params[0];
			for (std::map<Client*, bool>::iterator it3 = members.begin(); it3 != members.end(); ++it3)
				sendToClient(*it3->first, ":" + client.getNickname() + "!" + client.getUsername() + "@ircserv QUIT"
				+ reason + "\r\n");
		}
	}
	serv.disconnectClient(client.getFd());
}
