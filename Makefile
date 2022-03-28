NAME = webserv

HEADER = ./incs/

COMPILER = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC_PATH = srcs/

OBJ_PATH = obj/

SRC = main.cpp cluster.cpp conf_parser.cpp server.cpp

OBJ = $(addprefix $(OBJ_PATH), $(SRC:.cpp=.o))

all: $(OBJ_PATH) $(NAME)

$(NAME): $(OBJ)
					$(COMPILER) $(FLAGS) $(OBJ) -I $(HEADER) -o $(NAME) 
					@echo $(NAME) created

${OBJ_PATH}:
					@mkdir -p $@

${OBJ_PATH}%.o:		$(SRC_PATH)%.cpp
					$(COMPILER) $(FLAGS) -c $< -o $@

clean:
					/bin/rm -rf  $(OBJ_PATH)
					@echo Objects and libraries cleaned

fclean:				clean
					@/bin/rm -f $(NAME)
					@echo $(NAME) deleted

re:					fclean all

.PHONY:				all clean fclean re