#PROGRAM NAME
NAME = webserv

#COMPILATOR
CC = c++
CFLAGS = -Wall -Wextra -Werror -g -std=c++98 

#SOURCES
SRCSFD = srcs/
SRCS = \
		  main.cpp \
		  Server.cpp \
		  Location.cpp\
		  Request.cpp \
		  Response.cpp \
		  CgiHandler.cpp\
		  Config.cpp\
		  ConfigParser.cpp\
		  Client.cpp\
		  ServerManager.cpp

#OBJECTS
OBJSFD = objs/
OBJS = $(addprefix $(OBJSFD), $(SRCS:.cpp=.o))

#HEADERS
HDRSFD = includes/
HDR = \
	Library.hpp\
	CgiHandler.hpp \
	Config.hpp \
	Location.hpp \
	Request.hpp \
	Response.hpp \
	Server.hpp \
	Library.hpp \
	Client.hpp \
	ServerManager.hpp

HDRS = $(addprefix $(HDRSFD), $(HDR))
HDR_INC = -I./includes

#COLOURS
RED = \033[0;31m
GREEN = \033[0;32m
NONE = \033[0m

all: ${NAME}

${OBJSFD}:
		@mkdir $@
		@echo "\t[ $(GREEN)✔$(NONE) ] $@ directory"

${NAME}: ${OBJSFD} ${OBJS} ${HDRS}
		@${CC} ${CFLAGS} ${OBJS} -o $@
		@echo "\t[ $(GREEN)✔$(NONE) ] $@ executable"

${OBJSFD}%.o: ${SRCSFD}%.cpp
		@${CC} ${CFLAGS} ${HDR_INC} -o $@ -c $<

clean: 
		@/bin/rm -rf $(OBJSFD)
		@echo "\t[ $(RED)✗$(NONE) ] $(OBJSFD) directory"
fclean:	clean
		@/bin/rm -f $(NAME)
		@echo "\t[ $(RED)✗$(NONE) ] $(NAME) executable"

re:	fclean all

.PHONY: all clean fclean re 
