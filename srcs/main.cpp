/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:31:43 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/11 21:52:18 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Printers.hpp"

int g_sigint = 0;

void	ft_sigint_handler(int c)
{
	(void)c;
	g_sigint = 1;
	std::cout << MAGENTA << "\r[HALT]: STOPPING THE SERVER...\n";
}

int main(int ac, char **av)
{
	(void)ac;
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, ft_sigint_handler);

	std::string configPath(av[1] ? av[1] : "c.conf");
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