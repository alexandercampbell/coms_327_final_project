
CPP = clang++

CFLAGS = -std=gnu++11 -Wall -ggdb
LDFLAGS = -lncurses

BIN = main
OBJS = main.o io.o dungeon.o

.PHONY: all
all: $(BIN) ctags

$(BIN): $(OBJS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CPP) $(CFLAGS) -MMD -MF $*.d -c $<


.PHONY: clean
clean:
	rm -rf $(BIN) *.o *.d TAGS

ctags:
	ctags *.hpp *.cpp

