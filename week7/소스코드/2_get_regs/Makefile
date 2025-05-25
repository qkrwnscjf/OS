CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard *.c)
TARGETS = $(SRCS:.c=)

all: $(TARGETS)

% : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	rm -f $(TARGETS) *.o 
