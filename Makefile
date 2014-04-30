NAME = sip2smpp

include Makefile.defs

CONFIG_OBJ = OBJ

cleandirs = src OBJ
program_name = sip2smpp

#problem with first make

make:
	$(LD) $(CXXFLASG) $(CONFIG_OBJ)/*.o -o $(program_name) $(ARFLAGS)
	-@if [ ! -d "$(CONFIG_OBJ)" ]; then \
		mkdir $(CONFIG_OBJ) ; \
	fi ;
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
	-@for r in $(cleandirs) "" ; do \
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
	
