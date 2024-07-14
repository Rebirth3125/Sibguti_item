.PHONY: poker all election

all: main poker election

main:
	g++ src/main.cpp -I include -I external/bigint -o main -g

poker:
	g++ src/poker/client.cpp -I include -o poker

election:
	g++ src/election/client.cpp -I include -I external -o client -g
	g++ src/election/server.cpp -I include -I external -o server -g
