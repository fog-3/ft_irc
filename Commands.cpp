#include "Commands.hpp"

void	cmdPass(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		// TODO: send error
		return ;
	}
	if (msg.params[0] == serv.getPassword())
		client.setAuthenticated(true);
	else
	{
		// TODO: send error
		return ;
	}
}

void	cmdNick(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		// TODO: send error
		return ;
	}
	if (!client.getAuthenticated())
	{
		// TODO: send error
		return ;
	}
	if (serv.isNickTaken(msg.params[0]))
	{
		// TODO: send error
		return ;
	}
	client.setNickname(msg.params[0]);
	if (client.getUsername() != "")
		client.setRegistered(true);
}

void	cmdUser(Server &serv, Client &client, Message &msg)
{
	(void)serv;
	if (client.getUsername() != "")
	{
		// TODO: send error
		return ;
	}
	if (msg.params.empty())
	{
		// TODO: send error
		return ;
	}
	if (!client.getAuthenticated())
	{
		// TODO: send error
		return ;
	}
	client.setUsername(msg.params[0]);
	if (client.getNickname() != "")
		client.setRegistered(true);
}

void	cmdJoin(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel *chan = serv.findChannel(msg.params[0]);
	if (chan)
	{
		std::map<Client*, bool> members = chan->getMembers();
		if (members.find(&client) != members.end())
		{
			// TODO: send error
			return ;
		}
		if (chan->getInviteOnly())
		{
			std::vector<std::string>::iterator it = std::find(chan->getInviteList().begin(), chan->getInviteList().end(), client.getNickname());
			if (it == chan->getInviteList().end())
			{
				// TODO: send error
				return ;
			}
		}
		if (chan->getKey() != "" && (msg.params.size() < 2 || msg.params[1] != chan->getKey()))
		{
			// TODO: send error
			return ;
		}
		chan->addMember(&client);
		// TODO: send join notification
	}
	else
	{
		Channel *newChan = new Channel(msg.params[0]);
		serv.addChannel(newChan);
		newChan->addMember(&client);
		// TODO: send join notification
	}
}

void	cmdPrivmsg(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (msg.params[0][0] == '#' && chan)
	{
		std::map<Client*, bool> members = chan->getMembers();
		for (std::map<Client*, bool>::iterator it = members.begin(); it != members.end(); ++it)
		{
			// TODO: send message
		}
	}
	else
	{
		Client	*addr = serv.findClient(msg.params[0]);
		if (!addr)
		{
			// TODO: send error
			return ;
		}
		// TODO: send message
	}
}

void	cmdPart(Server &serv, Client &client, Message &msg)
{
	if (msg.params.empty())
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		// TODO: send error
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	if (members.find(&client) == members.end())
	{
		// TODO: send error
		return ;
	}
	chan->removeMember(&client);
}

void	cmdKick(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		// TODO: send error
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	std::map<Client *, bool>::iterator it = members.find(&client);
	if (it == members.end())
	{
		// TODO: send error
		return ;
	}
	if (!it->second)
	{
		// TODO: send error
		return ;
	}
	Client	*rem = serv.findClient(msg.params[1]);
	if (!rem)
	{
		// TODO: send error
		return ;
	}
	if (members.find(rem) != members.end())
		chan->removeMember(rem);
	else
	{
		// TODO: send error
		return ;
	}
}

void	cmdInvite(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[1]);
	if (!chan)
	{
		// TODO: send error
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	std::map<Client *, bool>::iterator it = members.find(&client);
	if (it == members.end())
	{
		// TODO: send error
		return ;
	}
	if (!it->second)
	{
		// TODO: send error
		return ;
	}
	Client	*inv = serv.findClient(msg.params[0]);
	if (!inv)
	{
		// TODO: send error
		return ;
	}
	chan->addToInviteList(inv->getNickname());
	// TODO: send invite notification to inv
}

void	cmdTopic(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 1)
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		// TODO: send error
		return ;
	}
	if (msg.params.size() < 2)
	{
		std::string	topic = chan->getTopic();
		if (topic.empty())
		{
			// TODO: send error
			return ;
		}
		// TODO: send topic
	}
	else
	{
		std::map<Client*, bool> members = chan->getMembers();
		std::map<Client *, bool>::iterator it = members.find(&client);
		if (it == members.end())
		{
			// TODO: send error
			return ;
		}
		if (!chan->getTopicRestricted() || it->second)
		{
			chan->setTopic(msg.params[1]);
		}
		else
		{
			// TODO: send error
			return ;
		}
	}
}

void	cmdMode(Server &serv, Client &client, Message &msg)
{
	if (msg.params.size() < 2)
	{
		// TODO: send error
		return ;
	}
	if (!client.getRegistered())
	{
		// TODO: send error
		return ;
	}
	Channel	*chan = serv.findChannel(msg.params[0]);
	if (!chan)
	{
		// TODO: send error
		return ;
	}
	std::map<Client*, bool> members = chan->getMembers();
	std::map<Client *, bool>::iterator it = members.find(&client);
	if (it == members.end())
	{
		// TODO: send error
		return ;
	}
	if (!it->second)
	{
		// TODO: send error
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
			// TODO: send error
			return ;
		}
		std::map<Client *, bool>::iterator it2 = members.find(rem);
		if (it2 == members.end())
		{
			// TODO: send error
			return ;
		}
		chan->setOperator(rem, true);
	}
	else if (msg.params[1] == "-o" && msg.params.size() == 3)
	{
		Client	*rem2 = serv.findClient(msg.params[2]);
		if (!rem2)
		{
			// TODO: send error
			return ;
		}
		std::map<Client *, bool>::iterator it3 = members.find(rem2);
		if (it3 == members.end())
		{
			// TODO: send error
			return ;
		}
		chan->setOperator(rem2, false);
	}
}
