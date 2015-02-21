INCLUDE=../../include
LIB=../../lib
BIN=../../bin

reader.tab.o:
	bison -d reader.ypp
	g++ -c reader.tab.cpp -I$(INCLUDE)

lex.yy.o:
	flex reader.l
	g++ -c lex.yy.c -I$(INCLUDE)

reader: reader.tab.o lex.yy.o
	g++ reader.tab.o lex.yy.o $(LIB)/Ponzi.o $(LIB)/Environment.o $(LIB)/Symbol.o -I$(INCLUDE) -o $(BIN)/reader

clean:
	rm -rf *.tab.[ch]pp
	rm -rf *.tab.o
	rm -rf lex.yy.*


