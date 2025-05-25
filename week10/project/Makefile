CC=gcc
CFLAGS=-g -o
TARGETS=ta_project
all: $(TARGETS)
.PHONY: all
%:
	$(CC) $(CFLAGS) $@ $@.c -lpthread
       
clean:
	rm $(TARGETS)

