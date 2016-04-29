# My Little Makefile
FLEX=flex
BISON=bison
CC=clang++
PARAM=-std=c++11 -fcxx-exceptions -Wno-deprecated-register
OUTPUT=p1

all: flex yacc clang

clang: *.cpp *.h
	${CC} -o ${OUTPUT} *.cpp ${LLVMPARAM} ${PARAM}

yacc: parser.y
	${BISON} --defines=parser.h --output=parser.cpp parser.y

flex: lex.l
	${FLEX} -o lex.cpp lex.l 

clean:
	rm parser.cpp parser.h lex.cpp ${OUTPUT}

test: all
	./${OUTPUT} < input_example

