include ../../../Makefile.defs

CONFIG_OBJ = ../../../OBJ

SRC= $(wildcard *.c)
OBJ += $(SRC:.c=.o)

.PHONY: all
all: CFLAGS+= 2> /dev/null
all: CXXFLAGS+= 2> /dev/null
all: build

.PHONY: debug
debug: CFLAGS+=
debug: CXXFLAGS+=
debug: build

.PHONY: build
build: $(OBJ)
%o: %c
	$(CC) -c $^ -o $(CONFIG_OBJ)/$@ $(CFLAGS)
%o: %cpp
	$(CXX) -c $^ -o $(CONFIG_OBJ)/$@ $(CXXFLAGS)

.PHONY: clean
clean:
	-@for r in $(OBJ) "" ; do \
                if [ -f "$(CONFIG_OBJ)/$$r" ]; then \
			echo "rm $(CONFIG_OBJ)/$$r" ; \
			rm -f $(CONFIG_OBJ)/$$r ; \
                fi ; \
        done
