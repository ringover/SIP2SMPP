include ../../Makefile.defs

CONFIG_OBJ = ../OBJ

subdirs = udp tcp sip smpp

SRC= $(wildcard *.c)
OBJ += $(SRC:.c=.o)

make: $(OBJ)

%o: %c
	$(CC) -c $^ -o $(CONFIG_OBJ)$@ $(CFLAGS)
%.o:
	-@for r in $(subdirs) "" ; do \
                if [ -n "$$r" ]; then \
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

