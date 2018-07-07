.PHONY : clean sxshell

OBJS = main.o\
		parse.o\
		init.o
CC=cc
CFLAGS=-Wall -g 

sxshell : $(OBJS)
	$(CC)  $(CFLAGS) -o $@ $(OBJS) -std=gnu99

%.o : %.c
	$(CC) -c $< -o $@ -std=gnu99

clean:
	rm -rf $(OBJS)

