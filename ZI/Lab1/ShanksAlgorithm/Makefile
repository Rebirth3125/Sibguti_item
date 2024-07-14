.PHONY: all

CFILE=ShanksAlgorithm

all: ${CFILE}

${CFILE}: ${CFILE}.o
	g++ -o ${CFILE} ${CFILE}.o -L./../../libs/Lab1/ -lbasicalgs -lm

${CFILE}.o: ${CFILE}.cpp
	g++ -I./../../libs -c ${CFILE}.cpp