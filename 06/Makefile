NAME = HackAssembler

CC = g++

RM = rm -rf

all: $(NAME)
	@echo "MAKE ALL IS DONE"

$(NAME):
	@$(CC) -Wall -o $(NAME) HackAssembler.cpp

clean:
	@$(RM) $(NAME) *.hack
	@echo "MAKE CLEAN IS DONE"

re: clean all
	@echo "MAKE RE IS DONE"

.PHONY: clean
