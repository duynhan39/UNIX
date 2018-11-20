# Dodge

Original UNIX game Dodge with C++, featuring graphics, database, leaderboard, and player modes 

### Why?

Started with a project for my UNIX class with a prompt from the professor *Make something that uses curses and gdbm. Grade based on impression*, I found myself get sucked in by its potential, of all the things I could come up with to add in this game. Even after my submission, I still continued developing, coming up with new ideas to make the game fun and *unpredictable*. I eventually slowed down on making new updates, but still have in mind a many more ideas to expand the game.

A simple game that applied the principle of Object Oriented Design in mind. It is constructed to be able to integrate with further development, functions, modes, etc. 

It could be a helpful tool to teach programming due to its simplicity and great potential. Dodge is easy to understand, easy to integrate, leaving room for everyone to meddle around with their own creativity.

### Prerequisites

First you have to install `gdbm` for database and `ncurses` for graphics. Open your terminal, run
For Mac:
```
brew install ncurses
brew install gdbm
```

## Getting Started

Clone the project to where you want to have it on your local machine

### Running

After cloning the project, inside the directory by
```
cd Dodge
```
For the first time running, type
```
make
```
Now, everytime you want to play, just have to type
```
./game
```

<img src="https://github.com/duynhan39/UNIX/blob/master/Dodge/Dodge.png" width="350">

**Enjoy the game!**

## Versions

* **Version 2.0:** Major update release.
  * Featuring **dual players mode**
  * Fixed minor bugs with database then caused it to run out of memory
  * Fixed speed transition to make its increasement smoother

## Author

* [**Nhan Cao**](https://www.linkedin.com/in/nhan-cao/)
