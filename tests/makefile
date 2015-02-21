INCLUDE=../include
LIB=../lib
BIN=../bin

tests: 
	cd ../src/repl; make clean reader; cd -
	g++ -I$(INCLUDE) test.cpp $(LIB)/Ponzi.o $(LIB)/Environment.o $(LIB)/Symbol.o -o test
	./test
	cat tests.scm | $(BIN)/reader 

clean:
	rm ./test
