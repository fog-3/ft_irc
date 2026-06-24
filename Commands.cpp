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
