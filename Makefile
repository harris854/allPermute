CC=g++ -pthread -std=c++11

CFLAGS=-c -Wall -pthread -std=c++11

all:testt

testt : main.o JobSimulator.o
	$(CC) main.o JobSimulator.o -o ./testt
	chmod 755 ./testt
main.o: main.cpp JobSimulator.h
	$(CC) $(CFLAGS) main.cpp
JobSimulator.o: JobSimulator.cpp JobSimulator.h
	$(CC) $(CFLAGS) JobSimulator.cpp
clean:
	rm -r *.o
	rm  testt

