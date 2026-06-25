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
		if (chan->getMembers().find(&client) != chan->getMembers().end())
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
	}
	else
	{
		Channel *newChan = new Channel(msg.params[0]);
		serv.addChannel(newChan);
		newChan->addMember(&client);
	}
}
