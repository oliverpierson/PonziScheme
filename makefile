INCLUDE=./include
LIB=./lib

objects = Ponzi.o Environment.o Symbol.o

all: $(objects)

Ponzi.o: 
	rm -f $(LIB)/Ponzi.o
	g++ -c src/Ponzi.cpp -o $(LIB)/Ponzi.o -I$(INCLUDE)

Environment.o: 
	rm -f $(LIB)/Environment.o
	g++ -c src/Environment.cpp -o $(LIB)/Environment.o -I$(INCLUDE)

Symbol.o: 
	rm -f $(LIB)/Symbol.o
	g++ -c src/Symbol.cpp -o $(LIB)/Symbol.o -I$(INCLUDE)

clean:
	rm -f $(LIB)/*.o
