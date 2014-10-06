# Derek Drummond and Sam Bruns
# PA2 - makefile
# Due: 09/12/14

CC = g++
FLAGS = -std=c++11 -pedantic
EXECS = awget ss

all: $(EXECS)

awget: awget.o
	$(CC) $(FLAGS) awget.o -o awget
	
ss: ss.o
	$(CC) $(FLAGS) ss.o -o ss
	
awget.o: awget.h awget.cpp
	$(CC) $(FLAGS) -c awget.cpp

ss.o: ss.cpp
	$(CC) $(FLAGS) -c ss.cpp
	
clean:
	rm -f *.o $(EXEC)
