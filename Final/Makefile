all: player_referee
	chmod +x *
	@echo Compiled Rock,paper,scissors!

player_referee: player.cpp referee.cpp
	g++ player.cpp -o player -std=c++11
	g++ referee.cpp -o referee -std=c++11

clean: 
	rm -f player referee
	rm -f player.exe referee.exe

