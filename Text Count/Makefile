
all: count-opens.o lsl.o
	g++ -Wall -o all count-opens lsl

# Compile with -g to retain debugging information

count-opens: g++ -std=c++11 -g -c -Wall count-opens.cpp -o count-opens
lsl: g++ -std=c++11 -g -c -Wall lsl.cpp -o lsl

