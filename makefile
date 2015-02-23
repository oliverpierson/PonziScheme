INCLUDE=./include
LIB=./lib

objects = Ponzi.o Environment.o Symbol.o

all: $(objects)

Ponzi.o: 
	rm -f $(LIB)/Ponzi.o
	g++ -std=c++11 -c src/Ponzi.cpp -o $(LIB)/Ponzi.o -I$(INCLUDE)

Environment.o: 
	rm -f $(LIB)/Environment.o
	g++ -std=c++11 -c src/Environment.cpp -o $(LIB)/Environment.o -I$(INCLUDE)

Symbol.o: 
	rm -f $(LIB)/Symbol.o
	g++ -std=c++11 -c src/Symbol.cpp -o $(LIB)/Symbol.o -I$(INCLUDE)

clean:
	rm -f $(LIB)/*.o
