
CC = clang

CFLAGS = -std=gnu11 -Wall -ggdb
LDFLAGS = -lncurses

BIN = main
OBJS = main.o

.PHONY: all
all: $(BIN) ctags

$(BIN): $(OBJS)

-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MF $*.d -c $<


.PHONY: clean
clean:
	rm -rf $(BIN) *.o *.d TAGS

ctags:
	ctags *.h *.c

