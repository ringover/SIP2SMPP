include ../../Makefile.defs

OBJ = daemonize.o

make: $(OBJ)

%o: %c
	$(CC) -c $^ -o $@ $(CFLAGS)
        
install:
        
clean:
	-@rm -f *.o

