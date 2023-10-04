# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/27 15:53:14 by nmaliare          #+#    #+#              #
#    Updated: 2023/09/19 20:57:47 by yli              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS = -Wall -Wextra -Werror -std=c++98 -g3
#-fsanitize=address

CC = c++

NAME = irc

SRC = main.cpp ./client/Client.cpp ./server/Server.cpp ./cmd/Cmd.cpp \
		./commands/pass/Pass.cpp ./commands/user/User.cpp ./commands/nick/Nick.cpp \
		./commands/mode/Mode.cpp ./commands/invite/Invite.cpp ./commands/join/Join.cpp ./commands/kick/Kick.cpp \
		./commands/privmsg/Privmsg.cpp ./commands/topic/Topic.cpp \
		./channel/Channel.cpp  ./commands/ping/Ping.cpp \
		./bot/Bot.cpp

OBJ = $(SRC:.cpp=.o)

# COLORS
NOC         = \033[0m
GREEN       = \033[1;32m
YELLOW      = \033[1;33m
BLUE        = \033[1;34m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)$(NAME) successfully compiled !$(NOC)\n"

$(OBJ): %.o:%.cpp
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "$(GREEN)[\342\234\205] $(notdir $@)\n";

clean:
	@if [ "$(wildcard $(addprefix ./, $(OBJ)))" ]; then\
		printf "$(BLUE)Cleaning $(NAME) files: $(YELLOW)$(OBJ)$(NOC) $(GREEN) [ \342\234\205 ] $(NOC)\n";\
		/bin/rm -f $(OBJ);\
	else\
		printf "$(YELLOW)There's no object files in $(NAME) \360\237\230\261$(NOC)\n";\
	fi

fclean: clean
	@if [ "$(wildcard $(addprefix ./, $(NAME)))" ]; then\
		printf "$(BLUE)Cleaning $(NAME): $(YELLOW)$(NAME) $(GREEN) [ \342\234\205 ]\n$(NOC)";\
		/bin/rm -f $(NAME);\
	else\
		printf "$(YELLOW)There's no $(GREEN)$(NAME) $(YELLOW)file here \360\237\230\261\n$(NOC)";\
	fi

re: fclean all

.PHONY:	all clean fclean re