CFLAGS  = `pkg-config --cflags --libs gtk+-3.0`

all: player_referee
	chmod +x *
	@echo Compiled Rock,paper,scissors!

player_referee: player.cpp referee.cpp
	g++ referee.cpp -o referee -std=c++11
	g++ player.cpp -o player -std=c++11
	g++ gui-player.cpp -o gui-player -std=c++11 ${CFLAGS}
clean: 
	rm -f player referee
	rm -f player.exe referee.exe

