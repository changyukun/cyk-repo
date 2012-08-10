CROSS_COMPILE:=
CC:=$(CROSS_COMPILE)gcc
GCC:=$(CROSS_COMPILE)g++

all:
	$(CC) -o client.out client.c stdsoap2.c soapC.c soapClient.c

clean:
	rm -rf client.out

