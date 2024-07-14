.PHONY: all

CFILE=Vernam

all: ${CFILE}

${CFILE}: ${CFILE}.o ./../../libs/Lab1/libbasicalgs.a
	g++ -o ${CFILE} ${CFILE}.o -L./../../libs/Lab1/ -lbasicalgs -lm

${CFILE}.o: ${CFILE}.cpp
	g++ -I./../../libs -c ${CFILE}.cpp