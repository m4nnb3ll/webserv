#include "Config.hpp"
#include "Printers.hpp"

int g_sigint = 0;

void	ft_sigint_handler(int c)
{
	(void)c;
	g_sigint = 1;
}

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, ft_sigint_handler);
	if (ac != 2)
		Printers::print_err("Wrong number of args!");
	
	std::string configPath(av[1]);
	try
	{
		Config conf(configPath);
		conf.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}