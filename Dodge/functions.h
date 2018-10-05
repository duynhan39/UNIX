#ifndef NHAN
#define NHAN

#include <curses.h>
#include <unistd.h>   // sleep
#include <string.h>
#include <iostream>
#include <stdlib.h>   // random
#include <time.h>     // time
#include <ctime>
#include <ratio>
#include <chrono>     // millisecond
#include <algorithm>  // min/max
#include <queue>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gdbm.h>

#define SLIDE_SP 100000
#define WID 30
#define COLOR_RANGE 3
#define GAP 100000
#define AC_GAP 200
#define LEVEL_SCORE 15
#define NAME_LEN 12
#define GAME_NAME "[DODGE]"
#define LIMIT 10  // Max level of speed
#define DATA_FILE "highscores.dat"

void GAME();
void greet();
void play();
void gameOver();
void printBorders(int end);
void printPlayer();
void printScore();
void printMess(std::string message, int loc=LINES/2);
void dropObjects();
void getName();
void printLeaderBoard();
int score();
int sleepTime();
void getMax();

void upDateDataBase();
void add_to_record(GDBM_FILE db, std::string player, std::string score);
//bool compareRec(const hsRec &a, const hsRec &b);
void printLeaderBoard();

#endif
