INCLUDE=./include

mceval.o: clean
	g++ -c src/fuji.cpp -o lib/fuji.o -I$(INCLUDE)

clean:
	rm -f lib/*.o
