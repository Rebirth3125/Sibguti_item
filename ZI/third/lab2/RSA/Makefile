BIN := RSA
LIB := ../../libs/

.PHONY: all clean

all: ${BIN}

clean:
	rm ${BIN}

${BIN}: ${BIN}.cpp
	g++ -I ./../../libs ${BIN}.cpp -o ${BIN} -lssl -lcrypto