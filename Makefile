CFLAGS= -Wall -Wextra -Werror -pedantic -std=c99
SRC= $(wildcard src/*.c)
OBJ= ${SRC:.c=.o}

all: minishell
minishell: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o minishell
clean:
	rm $(OBJ) minishell
