INCLUDE=./include

mceval.o: clean
	g++ -c src/ponzi.cpp -o lib/ponzi.o -I$(INCLUDE)

clean:
	rm -f lib/*.o
