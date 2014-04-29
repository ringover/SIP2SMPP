include ../Makefile.defs

subdirs = daemonize ini log sip smpp 
OBJ = sip2smpp.o connectionSIP.o connectionSMPP.o createMessageSip.o database.o parseSip.o

make: $(OBJ)
#	$(LD) $(CXXFLASG) $(NAME).cpp $(OBJ) -o $(NAME) $(ARFLAGS)

%o: %c
	$(CC) -c $^ -o $@ $(CFLAGS)
%o: %cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

%.o:
	make -C daemonize
	make -C ini
	make -C log
	make -C sip
	make -C smpp


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

