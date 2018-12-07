all: client_server
	chmod +x *
	@echo Compiled Rock,paper,scissors!

client_server: client.cpp server.cpp
	g++ client.cpp -o client -std=c++11
	g++ server.cpp -o server -std=c++11

clean: 
	rm -f client server
	rm -f client.exe server.exe

