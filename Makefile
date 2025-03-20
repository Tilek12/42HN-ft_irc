# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/12 12:44:18 by llacsivy          #+#    #+#              #
#    Updated: 2025/03/20 10:49:18 by llacsivy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= ircserv
CPP				:= c++
CPPFLAGS		:= -Wextra -Wall -Werror -std=c++17

SRCS			:= 	src/main.cpp \
					src/Channel.cpp

OBJS			:=	${SRCS:.cpp=.o}

all : $(NAME)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re