#include "Channel.hpp"

Channel::Channel() : _name(), _topic(), _members(), _inviteList(), _inviteOnly(false), _topicRestricted(false), _key(), _limit(-1)
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

void	Channel::setInviteOnly(bool set)
{
	this->_inviteOnly = set;
}

std::string	Channel::getKey() const
{
	return (this->_key);
}

void	Channel::setKey(std::string set)
{
	this->_key = set;
}

int	Channel::getLimit() const
{
	return (this->_limit);
}

void	Channel::setLimit(int set)
{
	this->_limit = set;
}

std::string	Channel::getName() const
{
	return (this->_name);
}

void	Channel::removeMember(Client *client)
{
	_members.erase(client);
}

void	Channel::addToInviteList(std::string nick)
{
	_inviteList.push_back(nick);
}

std::string	Channel::getTopic() const
{
	return (this->_topic);
}

void	Channel::setTopic(std::string set)
{
	this->_topic = set;
}

bool	Channel::getTopicRestricted() const
{
	return (this->_topicRestricted);
}

void	Channel::setTopicRestricted(bool set)
{
	this->_topicRestricted = set;
}

void	Channel::setOperator(Client *client, bool op)
{
	std::map<Client *, bool>::iterator it = _members.find(client);
	it->second = op;
}
