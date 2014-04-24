CC  = clang
CXX = clang++

#CC  = scan-build clang
#CXX = scan-build clang++

make:
	${CC} -Wall -pedantic -g -c lib/daemonize/daemonize.c -o lib/daemonize/daemonize.o
	${CC} -Wall -pedantic -g -c lib/minIni/minIni.c -o lib/minIni/minIni.o
	${CC} -Wall -pedantic -g -c sip/udp_methode.c -o sip/udp_methode.o
	${CC} -Wall -pedantic -g -c smpp/structSMPP.c -o smpp/structSMPP.o
	${CC} -Wall -pedantic -g -c smpp/smpp.c -o smpp/smpp.o
	${CC} -Wall -pedantic -g -c smpp/tcp.c -o smpp/tcp.o
	${CC} -Wall -pedantic -g -c log/log.c -o log/log.o
	${CC} -Wall -pedantic -g -c database.c -o database.o
	${CC} -Wall -pedantic -g -c iniFile.c -o iniFile.o
	${CXX} -Wall -pedantic -g -c createMessageSip.cpp -o createMessageSip.o
	${CXX} -Wall -pedantic -g -c parseSip.cpp -o parseSip.o
	${CXX} -Wall -pedantic -g -c connectionSIP.cpp -o connectionSIP.o
	${CXX} -Wall -pedantic -g -std=c++11 -c connectionSMPP.cpp -o connectionSMPP.o
	${CXX} -pedantic -Wvariadic-macros -std=c++11 -Wall -g main.cpp database.o smpp/structSMPP.o smpp/tcp.o smpp/smpp.o sip/udp_methode.o connectionSIP.o connectionSMPP.o createMessageSip.o parseSip.o -o sip2smpp -lsmpp34 -pthread -ldbi
#	${CXX} -pedantic -std=c++11 -Wall -g main.cpp database.o smpp/structSMPP.o smpp/tcp.o smpp/smpp.o sip/udp_methode.o connectionSIP.o connectionSMPP.o createMessageSip.o parseSip.o iniFile.o log/log.o -o sip2smpp -lsmpp34 -pthread -ldbi

install:
	cp sip2smpp /usr/bin/
	mkdir /etc/sip2smpp && cp config.ini /etc/sip2smpp/
	
clean:
	rm -f *.o
	rm -f sip/*.o
	rm -f smpp/*.o
	rm sip2smpp
