CC=gcc
LC_ALL=cs_CZ.utf8
CFLAGS=-O2 -g -std=c99 -Wall -pedantic -Wextra
LDFLAGS=-lm

default: main

main: main.o symtable.o lexical_analysis.o buffer.o parser.o prec_analysis.o prec_stack.o semantic.o generator.o token_handler.o exit.o generator_stack.o

main.o: main.c lexical_analysis.h symtable.h buffer.h parser.h prec_analysis.h prec_stack.h semantic.h generator.h token_handler.h exit.h generator_stack.h

lexical_analysis.o: lexical_analysis.c lexical_analysis.h

symtable.o: symtable.c symtable.h

prec_stack.o: prec_stack.c prec_stack.h

buffer.o: buffer.c buffer.h

parser.o: parser.c parser.h

prec_analysis.o: prec_analysis.c prec_analysis.h

semantic.o : semantic.c semantic.h

generator.o : generator.c generator.h

token_handler.o : token_handler.c token_handler.h

exit.o : exit.c 

generator_stack.o : generator_stack.c generator_stack.h

clean:
	rm -f *.o main



