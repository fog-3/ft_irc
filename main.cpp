#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3 || (atoi(argv[1]) <= 0 || atoi(argv[1]) > 65535))
	{
		std::cerr << "Wrong input." << std::endl;
		return (1);
	}
	Server	serv(atoi(argv[1]), argv[2]);
	serv.run();
	return (0);
}