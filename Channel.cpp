#include "Channel.hpp"

Channel::Channel() : _name(), _topic(), _members(), _inviteList(), _inviteOnly(false), _topicRestricted(false), _key(), _limit(0)
{
	std::cout << "Channel Default constructor has been called." << std::endl;
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