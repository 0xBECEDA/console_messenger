#CC specifies which compiler we're using
CC = gcc

#LINKER_FLAGS specifies the libraries we're linking against
LIB = -lpthread

all : server.o client.o

server.o:
	$(CC) server.c $(LIB) -o server

client.o:
	$(CC) client.c -o client
