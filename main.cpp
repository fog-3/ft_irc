#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3 || (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535))
	{
		std::cerr << "Wrong input." << std::endl;
		return (1);
	}
	try
	{
		Server serv(atoi(argv[1]), argv[2]);
		serv.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
