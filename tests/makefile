INCLUDE=../include
LIB=../lib

tests: 
	g++ $(LIB)/ponzi.o test.cpp -o test -I$(INCLUDE)
	./test

clean:
	rm ./test
