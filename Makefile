CC  = clang
CXX = clang++

#CC  = scan-build clang
#CXX = scan-build clang++

make:
	${CC} -Wall -pedantic -g -c daemonize/daemonize.c -o daemonize/daemonize.o
	${CC} -Wall -pedantic -g -c sip/udp_methode.c -o sip/udp_methode.o
	${CC} -Wall -pedantic -g -c smpp/structSMPP.c -o smpp/structSMPP.o
	${CC} -Wall -pedantic -g -c smpp/smpp.c -o smpp/smpp.o
	${CC} -Wall -pedantic -g -c smpp/tcp.c -o smpp/tcp.o
	${CC} -Wall -pedantic -g -c log/log.c -o log/log.o
	${CC} -Wall -pedantic -g -c database.c -o database.o
	${CC} -Wall -pedantic -g -c ini/iniFile.c -o ini/iniFile.o
	${CXX} -Wall -pedantic -g -c createMessageSip.cpp -o createMessageSip.o
	${CXX} -Wall -pedantic -g -c parseSip.cpp -o parseSip.o
	${CXX} -Wall -pedantic -g -c connectionSIP.cpp -o connectionSIP.o
	${CXX} -Wall -pedantic -g -std=c++11 -c connectionSMPP.cpp -o connectionSMPP.o
	${CXX} -pedantic -std=c++11 -Wall -g main.cpp database.o ini/iniFile.o log/log.o smpp/structSMPP.o smpp/tcp.o smpp/smpp.o sip/udp_methode.o daemonize/daemonize.o connectionSIP.o connectionSMPP.o createMessageSip.o parseSip.o -o sip2smpp -lsmpp34 -pthread -ldbi
#	${CXX} -pedantic -std=c++11 -Wall -g main.cpp database.o smpp/structSMPP.o smpp/tcp.o smpp/smpp.o sip/udp_methode.o connectionSIP.o connectionSMPP.o createMessageSip.o parseSip.o iniFile.o log/log.o -o sip2smpp -lsmpp34 -pthread -ldbi

install:
	cp sip2smpp /usr/bin/
	mkdir /etc/sip2smpp && cp config.ini /etc/sip2smpp/
	
clean:
	-@rm -f *.o
	-@for r in $(subdirs) "" ; do \
		if [ -n "$$r" ]; then \
			echo "" ; \
			echo "making $$r" ; \
			$(MAKE) -C $$r clean ; \
		fi ; \
	done
	rm sip2smpp
	
