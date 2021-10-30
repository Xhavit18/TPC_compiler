# Makefile PROJET FLEX-BISON

# $@ : the current target
# $^ : the current prerequisites
# $< : the first current prerequisite

CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall -lfl
EXEC = tpcc

bin/$(EXEC): obj/lex.yy.o obj/Projet.tab.o obj/parser.o obj/code_nasm.o obj/abstract-tree.o obj/SymbolTable.o obj/Main.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)
	@mkdir -p bin/
	@mv tpcc bin/
	@rm -f obj/abstract-tree.h
	@rm -f obj/abstract-tree.c

obj/lex.yy.o: obj/lex.yy.c obj/Projet.tab.h
	$(CC) -o lex.yy.o -c obj/lex.yy.c $(LDFLAGS)
	@mkdir -p obj/
	@mv lex.yy.o obj/

obj/Projet.tab.o: obj/Projet.tab.c obj/Projet.tab.h
	$(CC) -o Projet.tab.o -c obj/Projet.tab.c $(LDFLAGS)
	@mkdir -p obj/
	@mv Projet.tab.o obj/

obj/lex.yy.c: src/Projet.lex obj/Projet.tab.c obj/Projet.tab.h
	flex src/Projet.lex
	@mkdir -p obj/
	@mv lex.yy.c obj/
	@cp src/abstract-tree.h obj/
	@cp src/abstract-tree.c obj/

obj/Projet.tab.c obj/Projet.tab.h: src/Projet.y
	bison -d src/Projet.y
	@mkdir -p obj/
	@mv Projet.tab.c Projet.tab.h obj/

obj/Main.o: src/Main.c obj/SymbolTable.o obj/abstract-tree.o obj/code_nasm.o obj/parser.o
	$(CC) -o Main.o -c src/Main.c $(CFLAGS)
	@mkdir -p obj/
	@mv Main.o obj/

obj/abstract-tree.o: src/abstract-tree.c 
	$(CC) -o abstract-tree.o -c src/abstract-tree.c  $(CFLAGS)
	@mkdir -p obj/
	@mv abstract-tree.o obj/

obj/SymbolTable.o: src/SymbolTable.c 
	$(CC) -o SymbolTable.o -c src/SymbolTable.c  $(CFLAGS)
	@mkdir -p obj/
	@mv SymbolTable.o obj/

obj/code_nasm.o: src/code_nasm.c 
	$(CC) -o code_nasm.o -c src/code_nasm.c  $(CFLAGS)
	@mkdir -p obj/
	@mv code_nasm.o obj/

obj/parser.o: src/parser.c 
	$(CC) -o parser.o -c src/parser.c  $(CFLAGS)
	@mkdir -p obj/
	@mv parser.o obj/

clean:
	rm -f obj/*

vacuum: clean
	rm -f bin/*