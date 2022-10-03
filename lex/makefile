c_lex: main.o global.o lex.yy.o
	gcc -o c_lex main.o global.o lex.yy.o

main.o: main.c c_lexical_analyzer.h
	gcc -c main.c

global.o: global.c c_lexical_analyzer.h
	gcc -c global.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: c_lexical_analyzer.l c_lexical_analyzer.h
	lex c_lexical_analyzer.l

clean:
	rm *.o lex.yy.c c_lex