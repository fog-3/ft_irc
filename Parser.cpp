#include "Message.hpp"

std::string split(std::string &mes, size_t &start)
{
	if (start >= mes.size())
	{
		start = mes.size();
		return ("");
	}
	std::string	res;
	size_t	end = mes.find(" ", start);
	if (end == std::string::npos)
	{
		res = mes.substr(start);
		start = mes.size();
		return (res);
	}
	size_t	len = end - start;
	res = mes.substr(start, len);
	start = end + 1;
	return (res);
}

Message	Parser(std::string mes)
{
	if (mes.empty())
	{
		std::cerr << "Empty message." << std::endl;
		return Message();
	}
	Message	m;
	size_t	start = 0;
	size_t	n = mes.find("\r\n");
	if (n != std::string::npos)
		mes.erase(n);
	if (mes[0] == ':')
	{
		++start;
		m.prefix = split(mes, start);
	}
	m.command = split(mes, start);
	while(start < mes.size() && mes[start] != ':')
		m.params.push_back(split(mes, start));
	if (start < mes.size() && mes[start] == ':')
	{
		++start;
		m.params.push_back(mes.substr(start));
	}
	return (m);
}
