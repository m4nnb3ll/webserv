#ifndef FT_COMMON_H
# define FT_COMMON_H

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <sys/stat.h>
# include <map>
# include <netinet/in.h> // For struct sockaddr_in
# include <poll.h>
# include <arpa/inet.h> // Just for debug printing, remove later
# include <fcntl.h>
# include <unistd.h>
# include <cstdlib>
# include <signal.h>
# include <cstring>
# include <dirent.h>
#include <sys/wait.h>

# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define MAGENTA	"\033[0;35m"
# define CYAN		"\033[0;36m"
# define WHITE		"\033[0;37m"
# define RESET_COLOR "\033[0m"

# define BUFFER_SIZE 10000
// # define BUFFER_SIZE 4096

extern int g_sigint;

#endif