#PROGRAM NAME
NAME = webserv

#COMPILATOR
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

#SOURCES
SRCSFD = srcs/
SRCS = \
		  main.cpp \
		  Server.cpp \
		  Request.cpp \
		  Response.cpp \
		  CgiHandler.cpp\
		  Location.cpp\
		  Configuration.cpp\
		  ErrorPage.cpp

#OBJECTS
OBJSFD = objs/
OBJS = $(addprefix $(OBJSFD), $(SRCS:.cpp=.o))

#HEADERS
HDRSFD = includes/
HDR = \
	CgiHandler.hpp \
	Config.hpp \
	Location.hpp \
	ErrorPage.hpp \
	Request.hpp \
	Response.hpp \
	Server.hpp \

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
