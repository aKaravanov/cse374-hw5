CC = gcc
FLAGS = -g -Wall

main: dict_main.o dict.o
	$(CC) $(CFLAGS) -o main dict_main.o dict.o

dict_main.o: dict_main.c
	$(CC) $(CFLAGS) -c dict_main.c

dict.o: dict.c dict.h
	$(CC) $(CFLAGS) -c dict.c

clean:
	$(RM) main *.o *~

