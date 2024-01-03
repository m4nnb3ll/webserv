#COLORS
RED					:=	\033[0;31m
GREEN				:=	\033[0;32m
YELLOW				:=	\033[0;33m
RESET_COLOR			:=	\033[0m

NAME = webserv

CXXFLAGS = -Wall -Werror -Wextra -std=c++98
FSANITIZE = -g3 -fsanitize=address

FILES =	main.cpp \
		Printers.cpp \
		Config.cpp \
		Server.cpp \
		ServersSocket.cpp \
		Location.cpp \
		run.cpp \
		Client.cpp \
		Request.cpp \
		Response.cpp \
		utils/autoindex.cpp  \

INCLUDE_DIR = include

OBJS_DIR = objs
SRCS_DIR = srcs
SRCS = $(addprefix ${SRCS_DIR}/, $(FILES))
OBJS = $(addprefix ${OBJS_DIR}/, ${FILES:%.cpp=%.o})
RM		= rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	@c++ $(CXXFLAGS) -o $(NAME) $(OBJS) $(FSANITIZE)
	@echo "\n${GREEN}Done !${RESET_COLOR}"

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@mkdir -p ${@D}
	@printf "${YELLOW}Generate %-38.38s\r${RESET_COLOR}" $@
	@c++ $(CXXFLAGS) -I $(INCLUDE_DIR) $(FSANITIZE) -c $< -o $@

clean:
	@${RM} -r ${OBJS_DIR}
	@rm -f $(OBJS)
	@printf "${RED}Deleted objects\n${RESET_COLOR}"
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re