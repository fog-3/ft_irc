#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <map>

class Channel
{
	public:
	Channel();
	Channel(std::string name);
	Channel(Channel const &copy);
	Channel &operator=(Channel const &copy);
	~Channel();

	void						addMember(Client *client);
	std::map<Client*, bool>		getMembers() const;
	std::vector<std::string>	getInviteList() const;
	bool						getInviteOnly() const;
	void						setInviteOnly(bool set);
	std::string					getKey() const;
	void						setKey(std::string set);
	int							getLimit() const;
	void						setLimit(int set);
	std::string					getName() const;
	void						removeMember(Client *client);
	void						addToInviteList(std::string nick);
	std::string					getTopic() const;
	void						setTopic(std::string set);
	bool						getTopicRestricted() const;
	void						setTopicRestricted(bool set);
	void						setOperator(Client *client, bool op);
	
	private:
	std::string 				_name;
	std::string 				_topic;
	std::map<Client*, bool>		_members;
	std::vector<std::string>	_inviteList;
	bool 						_inviteOnly;
	bool 						_topicRestricted;
	std::string 				_key;
	int 						_limit;
};

#endif
