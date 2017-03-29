NAME=batnot
CFLAGS=-g -Wall -o $(NAME) -lupower-glib
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-3.0`
SRCS=batnot.c batnot-app.c batnot-util.c
CC=gcc

all: batnot

batnot: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(GTKFLAGS)

clean:
	rm $(NAME)
