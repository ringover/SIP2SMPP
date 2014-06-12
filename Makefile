NAME = sip2smpp

include Makefile.defs

CONFIG_OBJ = OBJ
ARG+=

subdirs= src
cleandirs = src OBJ

program_name = sip2smpp
bin_target = /usr/bin
cfg_target = /etc/sip2smpp

.DEFAULT_GOAL:=all

.PHONY: install
install: all
install:
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "" ;
	@ echo "#############################################################" ;
	@ echo "################           SIP2SMPP            ##############" ;
	@ echo "#############################################################" ;
	@ echo "#### INSTALL OK !" ;
	@ echo "#### " ;
	-@if [ -f "$(program_name)" ]; then \
		cp sip2smpp $(bin_target)/sip2smpp ; \
	fi ;
	-@if [ ! -d "$(cfg_target)" ]; then \
		echo "#### A INI File sample is created to $(cfg_target)/config.ini" ; \
		mkdir $(cfg_target) ; \
		cp `pwd`/config.ini $(cfg_target)/config.ini ; \
	fi ;
	@ echo "#### " ;
	@ echo "#### " ;
	@ echo "#### Run sip2smpp with :" ;
	@ echo "#### $(bin_target)/sip2smpp -c $(cfg_target)/config.ini" ;
	@ echo "#### " ;
	@ echo "#### Debug :" ;
	@ echo "#### $(bin_target)/sip2smpp -c $(cfg_target)/config.ini -D 8" ;
	@ echo "#### " ;
	@ echo "#############################################################" ;
	@ echo "#### " ;

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
	-@if [ ! -d "$(CONFIG_OBJ)" ]; then \
		mkdir $(CONFIG_OBJ) ; \
	fi ;
	-@for r in $(subdirs) "" ; do \
                if [ -n "$$r" ]; then \
			echo "Making $(ARG) $$r" ; \
                        $(MAKE) $(ARG) -C $$r ; \
                fi ; \
        done
	$(LD) $(CXXFLASG) $(CONFIG_OBJ)/*.o -o $(program_name) $(ARFLAGS) 2> /dev/null

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
	-@if [ -f "$(program_name)" ]; then \
		echo "rm $(program_name)" ; \
		rm -f $(program_name) ; \
	fi ;


