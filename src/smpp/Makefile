include ../../Makefile.defs

OBJ = smpp.o structSMPP.o tcp.o 

make: $(OBJ)

%o: %c
	$(CC) -c $^ -o $@ $(CFLAGS)
  
install:
        
clean:
	-@rm -f *.o

