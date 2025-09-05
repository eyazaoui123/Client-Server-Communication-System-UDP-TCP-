CC = gcc
CFLAGS = -Wall

all: clientTCP serveurTCP clientUDP serveurUDP 


TCP: clientTCP serveurTCP
UDP: clientUDP serveurUDP




clientTCP: clientTcpInt.c
	gcc clientTcpInt.c -o clientTCP `pkg-config --cflags --libs gtk+-3.0`

serveurTCP: serveurTCP.c
	gcc serveurTCP.c -o serveurTCP -Wall

clientTCPrun: clientTcpInt.c
	./clientTCP 
	
serveurTCPrun: serveurTCP.c
	./serveurTCP 3003
			
clientUDP: clientUDP.c
	gcc clientUDP.c -o clientUDP -Wall
	
serveurUDP: serveurUDP.c
	gcc serveurUDP.c -o serveurUDP -Wall
	
	
	
serveurUDPrun: serveurUDP.c
	./serveurUDP
	
clientUDPrun: clientUDP.c
	./clientUDP
	

clean:
	rm -f clientTCP serveurTCP


