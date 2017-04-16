
CPP = clang++

CFLAGS = -std=c++11 -Wall -ggdb
LDFLAGS = -lncurses

BIN = main
OBJS = main.o io.o world.o pc.o mob.o item.o

.PHONY: all
all: $(BIN) ctags

$(BIN): $(OBJS)
	$(CPP) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CPP) $(CFLAGS) -MMD -MF $*.d -c $<


.PHONY: clean
clean:
	rm -rf $(BIN) *.o *.d TAGS

ctags:
	ctags *.hpp *.cpp

