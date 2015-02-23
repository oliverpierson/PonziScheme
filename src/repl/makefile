INCLUDE=../../include
LIB=../../lib
BIN=../../bin

reader.tab.o:
	bison -d reader.ypp
	g++ -std=c++11 -c reader.tab.cpp -I$(INCLUDE)

lex.yy.o:
	flex reader.l
	g++ -std=c++11 -c lex.yy.c -I$(INCLUDE)

reader: reader.tab.o lex.yy.o
	g++ -std=c++11 reader.tab.o lex.yy.o $(LIB)/Ponzi.o $(LIB)/Environment.o $(LIB)/Symbol.o -I$(INCLUDE) -o $(BIN)/reader

clean:
	rm -rf *.tab.[ch]pp
	rm -rf *.tab.o
	rm -rf lex.yy.*


