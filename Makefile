make:


	clang -Wall -pedantic -g -c sip/udp_methode.c -o sip/udp_methode.o
	clang++ -Wall -pedantic -g -c createMessageSip.cpp -o createMessageSip.o
	clang++ -Wall -pedantic -g -c parseSip.cpp -o parseSip.o
	clang++ -Wall -pedantic -g -c connectionSIP.cpp -o connectionSIP.o
	clang++ -Wall -pedantic -g -std=c++11 -c connectionSMPP.cpp -o connectionSMPP.o

	clang++ -pedantic -std=c++11 -Wall -g main.cpp -o sip2smpp smpp/tcp.o smpp/smpp.o sip/udp_methode.o connectionSIP.o connectionSMPP.o createMessageSip.o parseSip.o -lsmpp34 -pthread

install:
	cp smsc /usr/bin/
	mkdir /etc/sip2smpp && cp config.ini /etc/sip2smpp/
	
clean:
	rm -f *.o
