CC = gcc
CFLAGS = -Wall -g --save-temp -o
TARGETS = tracee tracer
all: $(TARGETS)
.PHONY: all
%:
	$(CC) $(CFLAGS) $@ $@.c

clean: 
	rm -f $(TARGETS) *.o *.i *.s
