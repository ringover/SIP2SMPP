NAME = sip2smpp

include Makefile.defs

subdirs = src
program_name = sip2smpp
OBJ = src/*.o src/*/*.o

make: $(OBJ)
	$(LD) $(CXXFLASG) $(OBJ) -o $(program_name) $(ARFLAGS)

%.o:
	make -C src


install:
	cp -f $(program_name) /usr/bin/
	-@if [ -f "/etc/$(program_name)/config.ini" ]; then \
		echo "" ; \
		echo "cp config.ini" ; \
		mkdir /etc/$(program_name) && cp config.ini /etc/$(program_name)/; \
	fi ;

clean:
	-@rm -f *.o
	-@for r in $(subdirs) "" ; do \
		if [ -n "$$r" ]; then \
			echo "making $$r" ; \
			$(MAKE) -C $$r clean ; \
		fi ; \
	done
	-@if [ -f $(program_name) ]; then  \
		echo "" ; \
		echo "rm $(program_name)" ; \
		rm $(program_name); \
	fi ;
	
