INCLUDE=./include

all: Ponzi.o Environment.o Symbol.o

Ponzi.o: 
	rm -f lib/Ponzi.o
	g++ -c src/Ponzi.cpp -o lib/Ponzi.o -I$(INCLUDE)

Environment.o: 
	rm -f lib/Environment.o
	g++ -c src/Environment.cpp -o lib/Environment.o -I$(INCLUDE)

Symbol.o: 
	rm -f lib/Symbol.o
	g++ -c src/Symbol.cpp -o lib/Symbol.o -I$(INCLUDE)

clean:
	rm -f lib/*.o
