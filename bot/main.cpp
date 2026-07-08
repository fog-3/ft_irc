#include "Bot.hpp"

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Error: wrong number of params passed to activate the bot." << std::endl;
		return (1);
	}
	connectToServer(argv[1], argv[2], argv[3]);
}
