#include "functions.h"

using namespace std;

int pos;
queue<int> objLoc;
int level;
string playerName;
int maxOfPlayer;

struct hsRec { std::string name; int score; };

void GAME()
{
  refresh();

  greet();

  play();

  clear();
}

void greet()
{ 
  char s[COLS];
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
  
  use_default_colors();
  init_pair(1, COLOR_CYAN, -1);
  init_pair(2, COLOR_GREEN, -1);
  init_pair(3, COLOR_YELLOW, -1);
  init_pair(4, COLOR_RED, -1);
  init_pair(5, COLOR_WHITE, -1);
  init_pair(6, COLOR_MAGENTA, -1);
  init_pair(7, COLOR_BLUE, -1);

  init_pair(8, COLOR_RED, COLOR_RED);
  init_pair(9, COLOR_BLUE, COLOR_BLUE);

  init_pair(10, COLOR_BLACK, COLOR_YELLOW);
  init_pair(11, COLOR_BLACK, COLOR_WHITE);
  init_pair(12, COLOR_CYAN, COLOR_BLUE);
  init_pair(13, COLOR_MAGENTA, COLOR_GREEN);

  for(int rep=0; rep<=LINES+1; rep++)
  {
    clear();
    attrset(COLOR_PAIR(3));
    printMess(GAME_NAME);
    
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();

    srand(time(NULL));
    int seed = rand()%100 + 1; 
    usleep(seed*rep % 300);
    chrono::duration<double, milli> time_span = t2 - t1;

    int dens=1;
    for(int y=0; y<=(LINES-rep)/dens; y++)
    {
	for (int x = 0; x <= COLS/dens; x++)
	{
	    srand(time_span.count()*1000);
	    seed = rand()%100 + 1; 

	    int random = (x+1)*(y+1)*seed;
	    while(random%7 == 0)
	    {
		srand(random);
		seed = rand()%100 + 1;
		random -= seed;
	    }
	    if(x*y % 7 == 1)
	      attrset(COLOR_PAIR(random%COLOR_RANGE + 1) | A_BOLD);
	    else
	      attrset(COLOR_PAIR(random%COLOR_RANGE + 1));
      
	    srand(random);
	    sprintf(s, "%c", '0' + (rand() % 10) );

	    mvprintw(y*dens, x*dens, s);
      
	  }
      }
    refresh();
    move(0,0);
    usleep(SLIDE_SP*1/3);
  }
  
  // Waiting for user to read the title
  usleep(SLIDE_SP);
  
  // Moving the name down along with the borders
  for(int y=0; y<=LINES/2-1; y++)
  {
    if(y%2 == 0)
      attrset(COLOR_PAIR(y%7 + 1) | A_BOLD);
    else
      attrset(COLOR_PAIR(y%7 + 1));
    
    printMess(GAME_NAME, LINES/2+y);

    printBorders(y);
    
    usleep(SLIDE_SP*3/2);
    refresh();
    clear();
  }

  // Finishing the rest of the border
  for(int y=0; y<=LINES/2-1; y++)
  {
    printBorders(y+LINES/2);
    usleep(SLIDE_SP*3/2);
    refresh();
    clear();
  }
}

void printBorders(int end=LINES)
{
  attrset(COLOR_PAIR(3) | A_BOLD);
  
  for(int y=0; y<end; y++)
  {
    move(y, (COLS-WID-1)/2);
    printw("||");
    move(y, (COLS+WID+1)/2);
    printw("||");
    move(0,0);
  }
  refresh();
  
}

void printScore()
{
  attrset(COLOR_PAIR(2));
  //int score = score();
  string message = "SCORE: " + to_string( score() );
  move(2, (COLS-WID)/2 - 15);
  printw("%s", message.c_str());
  move(0,0);
  refresh();
}

void printPlayer()
{
  attrset(COLOR_PAIR(1));
   
  move(LINES-1, pos);
  printw("@");

  printBorders();
  printScore();
}

void printMess(string message, int loc)
{  
  move(loc, ( COLS-message.size()+1 )/2);
  printw("%s", message.c_str());

  move(0,0);
  refresh();
}

/////////////////////////
void play()
{
  // Init Game Data
  pos = COLS/2;
  level = -LINES+1;

  clear();
  printBorders();
  getName();
  getMax();

  clear();
  attrset(COLOR_PAIR(3));
  printMess("Only move");
  printMess("if you are truely prepared", LINES/2+1);
  printPlayer();

  // Game On!
  keypad(stdscr, TRUE);
  int key;
  key = getch();
  while(/*key != ERR &&*/ key != 'q')
  {
    nodelay(stdscr, TRUE);
    
    if(key == KEY_RIGHT)
      pos = min(pos+1, (COLS+WID)/2-1);
    else if(key == KEY_LEFT)
      pos = max(pos-1, (COLS-WID)/2+1);

    clear();

    dropObjects();
    printPlayer();
    key = getch();
  }
  if(key == 'q')
    gameOver();
}

void getName()
{
  attrset(COLOR_PAIR(7));
  char name[NAME_LEN+1];
  printMess("[Tell me your name, mortal]");
  move(LINES/2+1, (COLS-NAME_LEN)/2);
  getnstr(name, NAME_LEN);
  
  string nameStr(name);
  playerName = nameStr;
}

void dropObjects()
{
  srand( time(NULL)*(level/5) );
  objLoc.push(rand() % WID);

  while(objLoc.size() >= LINES)
    objLoc.pop();

  attrset(COLOR_PAIR(8));
  
  queue<int> temQue = objLoc;
  int count=-1;
  clear();
  while(!temQue.empty())
  {
    count++;

    // Check if hit player
    if(objLoc.size()-count == LINES-1 && (COLS-WID)/2 + temQue.front() == pos)
    {
      sleep(1);
      gameOver();
    }
      
    move(objLoc.size()-count, (COLS-WID)/2 + temQue.front());
    printw(" ");
    temQue.pop();
  }
  printPlayer();
  refresh();
  usleep(max( GAP*LIMIT/100, sleepTime() ));

  level++;
}

int score() { return max(0,level)/LEVEL_SCORE + 1; }
int sleepTime() { return level >= AC_GAP*(LIMIT-1) ? GAP/LIMIT : AC_GAP*GAP/(AC_GAP+max(0, level)) ; }  

void gameOver()
{
  clear();
  attrset( COLOR_PAIR(3) );
  printMess( "YOUR SCORE: " + to_string(score()) );
  sleep(1);

  clear();
  attrset(COLOR_PAIR(3));

  if(score() > maxOfPlayer)
    upDateDataBase();

  printLeaderBoard();
  sleep(1);
  
  clear();
  printMess("[Goodbye]");
  sleep(1);

  clear();
  endwin();
  exit(1);
}

void upDateDataBase()
{
  // First time player
  if (FILE *f = fopen(DATA_FILE, "r"))
  {
    fclose(f);
  }
  // Updating
  else
  {
    GDBM_FILE fG; 	
    fG = gdbm_open(DATA_FILE, 512, GDBM_WRCREAT, 0666, 0);
    gdbm_close(fG);
  }
  
  GDBM_FILE f;
  f = gdbm_open(DATA_FILE, 512, GDBM_WRCREAT, 0666, 0);
  add_to_record(f, playerName, to_string( score() ));
  gdbm_close(f);
}

void add_to_record(GDBM_FILE db, string player, string score)
{
  datum name, data; // Key and value (name and score).

  char *nameR = new char[player.length() + 1];
  strcpy(nameR, player.c_str());
  
  char *scoreR = new char[score.length() +1];
  strcpy(scoreR, score.c_str()); 
  
  // Adding the player name and his max score
  name.dptr = nameR;
  name.dsize = strlen(nameR);

  data.dptr = scoreR;
  data.dsize = strlen(scoreR);

  // Storing
  gdbm_store(db, name, data, GDBM_REPLACE);
  
  delete [] nameR;
  delete [] scoreR;
}

void getMax()
{
  GDBM_FILE f;
  datum pName, maxScore;

  char *nameR = new char[playerName.length() + 1];
  strcpy(nameR, playerName.c_str());

  pName.dptr = nameR;
  pName.dsize = strlen(nameR);
  
  f = gdbm_open(DATA_FILE, 512, GDBM_WRCREAT, 0666, 0);

  maxScore = gdbm_fetch(f, pName);

  maxScore.dptr == NULL? maxOfPlayer = -1 : maxOfPlayer = atoi(maxScore.dptr); 

  gdbm_close(f);
}

bool compareRec(const hsRec &a, const hsRec &b)
{ return a.score > b.score; }

void printLeaderBoard()
{
  GDBM_FILE f;
  datum current_key, current_data;
   
  f = gdbm_open(DATA_FILE, 512, GDBM_WRCREAT, 0666, 0);

  vector<hsRec> db;

  current_key = gdbm_firstkey(f);
  while(current_key.dptr != NULL)
  {
    // Fetching data out of database
    current_data = gdbm_fetch(f, current_key);
       
    if(current_data.dptr != NULL)
    { 
      struct hsRec curr;
      curr.name = current_key.dptr;
      curr.score = atoi(current_data.dptr);

      db.push_back(curr);
    }
    current_key = gdbm_nextkey(f, current_key);
  }
     
  // Taking top scores
  sort(db.begin(), db.end(), compareRec); 

  printMess("===Leader Board===");
  
  int listSize = min(4, (int)db.size() );
  for(int i=listSize-1; i>=0; i--)
  {
    string line(NAME_LEN-db[i].name.size()+6-to_string(db[i].score).size(), ' ');
    line = db[i].name + line + to_string(db[i].score);

    printMess(line, LINES/2+i+1);
    sleep(1);
  }
  
  refresh();
  sleep(3);

  gdbm_close(f);
}
