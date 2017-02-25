NAME=batnot
CFLAGS=-g -Wall -o $(NAME)
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-3.0`
SRCS=batnot.c batnot-app.c batnot-window.c batnot-util.c
CC=gcc

all: batnot

batnot: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(GTKFLAGS)

clean:
	rm $(NAME)
