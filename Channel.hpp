#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <map>

class Channel
{
	public:
	Channel();
	Channel(Channel const &copy);
	Channel &operator=(Channel const &copy);
	~Channel();
	
	private:
	std::string _name;
	std::string _topic;
	std::map<Client*, bool> _members;
	std::vector<std::string> _inviteList;
	bool _inviteOnly;
	bool _topicRestricted;
	std::string _key;
	int _limit;
};

#endif