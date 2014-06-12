include ../../Makefile.defs

CONFIG_OBJ = ../../OBJ

SRC= $(wildcard *.c)
OBJ += $(SRC:.c=.o)
ARG+=

.PHONY: all
all: CFLAGS+= 2> /dev/null
all: CXXFLAGS+= 2> /dev/null
all: ERG+=all
all: build

.PHONY: debug
debug: CFLAGS+=
debug: CXXFLAGS+=
debug: ARG+= debug
debug: build

.PHONY: build
build: $(OBJ)
	-@for r in $(subdirs) "" ; do \
                if [ -n "$$r" ]; then \
			echo "Making $(ARG) $$r" ; \
                        $(MAKE) $(ARG) -C $$r ; \
                fi ; \
        done
%o: %c
	$(CC) -c $^ -o $(CONFIG_OBJ)/$@ $(CFLAGS)
%o: %cpp
	$(CXX) -c $^ -o $(CONFIG_OBJ)/$@ $(CXXFLAGS)
 
.PHONY: clean
clean:
	-@for r in $(subdirs) "" ; do \
                if [ -n "$$r" ]; then \
			echo "Making clean $$r" ; \
                        $(MAKE) clean -C $$r ; \
                fi ; \
        done
	-@for r in $(OBJ) "" ; do \
                if [ -f "$(CONFIG_OBJ)/$$r" ]; then \
			echo "rm $(CONFIG_OBJ)/$$r" ; \
			rm -f $(CONFIG_OBJ)/$$r ; \
                fi ; \
        done
