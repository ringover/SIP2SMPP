include ../../Makefile.defs

CONFIG_OBJ = ../../OBJ

SRC= $(wildcard *.c)
OBJ += $(SRC:.c=.o)

make: $(OBJ)

%o: %c
	$(CC) -c $^ -o $(CONFIG_OBJ)/$@ $(CFLAGS)
        
install:
        
clean:
	-@rm -f *.o

