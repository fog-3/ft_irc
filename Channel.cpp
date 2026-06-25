#include "Channel.hpp"

Channel::Channel() : _name(), _topic(), _members(), _inviteList(), _inviteOnly(false), _topicRestricted(false), _key(), _limit(0)
{
	std::cout << "Channel Default constructor has been called." << std::endl;
}

Channel::Channel(std::string name) : _name(name), _topic(), _members(), _inviteList(), _inviteOnly(false), _topicRestricted(false), _key(), _limit(-1)
{
	std::cout << "Channel " << name << " created."<< std::endl;
}

Channel::Channel(Channel const &copy) : _name(copy._name), _topic(copy._topic), _members(copy._members), _inviteList(copy._inviteList), _inviteOnly(copy._inviteOnly), _topicRestricted(copy._topicRestricted), _key(copy._key), _limit(copy._limit)
{
	std::cout << "Channel Copy constructor has been called." << std::endl;
}

Channel &Channel::operator= (Channel const &copy)
{
	std::cout << "Channel assingment operator has been called." << std::endl;
	if (this != &copy)
	{
		_name = copy._name;
		_topic = copy._topic;
		_members = copy._members;
		_inviteList = copy._inviteList;
		_inviteOnly = copy._inviteOnly;
		_topicRestricted = copy._topicRestricted;
		_key = copy._key;
		_limit = copy._limit;
	}
	return (*this);
}

Channel::~Channel()
{
	std::cout << "Channel destructor has been called." << std::endl;
}

void	Channel::addMember(Client *client)
{
	if (_members.empty())
		_members[client] = true;
	else
		_members[client] = false;
}

std::map<Client*, bool>	Channel::getMembers() const
{
	return (this->_members);
}

std::vector<std::string>	Channel::getInviteList() const
{
	return (this->_inviteList);
}

bool	Channel::getInviteOnly() const
{
	return (this->_inviteOnly);
}

std::string	Channel::getKey() const
{
	return (this->_key);
}

int	Channel::getLimit() const
{
	return (this->_limit);
}

std::string	Channel::getName() const
{
	return (this->_name);
}