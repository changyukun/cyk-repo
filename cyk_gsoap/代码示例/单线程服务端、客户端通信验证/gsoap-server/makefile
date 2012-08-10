CROSS_COMPILE:=
CC:=$(CROSS_COMPILE)gcc
GCC:=$(CROSS_COMPILE)g++

all:
	$(CC) -o server.out server.c stdsoap2.c soapC.c soapServer.c

clean:
	rm -rf server.out