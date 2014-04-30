include ../Makefile.defs

CONFIG_OBJ = ../OBJ

subdirs = daemonize ini log net 

SRC = $(wildcard *.c*)
OBJ += $(SRC:.c=.o) $(SRC:.cpp=.o)

make: $(OBJ)
#	$(LD) $(CXXFLASG) $(NAME).cpp $(OBJ) -o $(NAME) $(ARFLAGS)

%o: %c
	$(CC) -c $^ -o $(CONFIG_OBJ)/$@ $(CFLAGS)
%o: %cpp
	$(CXX) -c $^ -o $(CONFIG_OBJ)/$@ $(CXXFLAGS)

%.o:
	-@for r in $(subdirs) "" ; do \
                if [ -n "$$r" ]; then \
			echo "Making $$r" ; \
                        $(MAKE) -C $$r ; \
                fi ; \
        done


install:
	
clean:
	-@rm -f *.o
	-@for r in $(subdirs) "" ; do \
		if [ -n "$$r" ]; then \
			echo "" ; \
			echo "making $$r" ; \
			$(MAKE) -C $$r clean ; \
		fi ; \
	done

