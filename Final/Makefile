CFLAGS  = `pkg-config --cflags --libs gtk+-2.0`

all: player_referee
	chmod +x *
	@echo Compiled Rock,paper,scissors!

player_referee: player.cpp referee.cpp gui-player.c
	g++ referee.cpp -o referee -std=c++11
	g++ player.cpp -o player -std=c++11
	gcc gui-player.c -o gui-player ${CFLAGS}

clean: 
	rm -f player referee
	rm -f player.exe referee.exe

