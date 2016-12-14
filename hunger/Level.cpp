#include "Level.h"
#include "separate.h"

#include <curses.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

using namespace std;

//Format:
/* <levelName>,<numRows>,<numFood>
 * <int[0][0]>,<int[0][1]>, ect..
 * <int[1][0]>,<int[1][1]>, ect..
 * ect..
 **/
Level::Level(string filename) 
{
	// Open the file
	ifstream fin;
	fin.open(filename);

	// Get the meta info
	string line;
	getline(fin, line);
	vector<string> sep_line = separate(line, ',');

	this->name = sep_line[0];
	int numRows = atoi(sep_line[1].c_str());
	this->numFood = atoi(sep_line[2].c_str());

	// Get actual map
	vector<vector<int>> map;
	for (int i = 0; i < numRows; i++)
	{

		// Get a row
		getline(fin, line);
		sep_line = separate(line, ',');

		vector<int> intRow;
		for (int j = 0; j < sep_line.size(); j++)
		{
			intRow.push_back(atoi(sep_line[j].c_str()));	
		}
		map.push_back(intRow);	
	}

	this->map = map;

}

int
Level::getNumFood() {return this->numFood;}


// The code for maps:
//
// 0 = regular ground
// 1 = start pos
// 2 = goal point
// 3 = track for goal
// 4
// 5
// 6
// 7
// 8
// 9 = wall, impenetrable
void
Level::play() 
{
	int nLoop = 0;

	int food = getNumFood();

	// Find start position(s)
	int x=-1; int y=-1;
	int f_x=-1; int f_y=-1;
	for (int i = 0; i < this->map.size(); i++) 
	{
		for (int j = 0; j < this->map[i].size(); j++) 
		{
			if (this->map[i][j] == 1) {x = i; y = j;}
			if (this->map[i][j] == 2) {f_x = i; f_y = j;}
		}
	}

	// If no start is found, tell 'em
	if (x == -1 || y == -1) 
	{
		printf("No start found!\n");
		return;
	}

	// Initialize screen
	initscr();
	int r, c, ch; char p = 254;
	getmaxyx(stdscr, r, c);

	// Until goal is reached
	// or dead
	while (this->map[x][y] != 2 && food != 0) 
	{
		
		// Print food left
		mvprintw(2,2,"Food Left: %d, nLoop: %d", food, nLoop);

		// Print map
		for (int i = 0; i < this->map.size(); i++)
		{
			for (int j = 0; j < this->map[i].size(); j++) 
			{
				int relx = (r/2) + i - x;
				int rely = (c/2) + j - y;
				if (relx < 0 || relx > r || rely < 0 || rely > c-1) {continue;}
				move(relx, rely);

				if (!(relx == r/2 && rely == c/2)) 
				{
					switch (this->map[i][j]) 
					{
						case 0: break;
						case 1: printw("1"); break;
						case 2: printw("2"); break;
						case 3: printw("+"); break;
						case 4: printw(""); break;
						case 5: printw(""); break;
						case 6: printw(""); break;
						case 7: printw(""); break;
						case 8: printw(""); break;
						case 9: printw("X");break;
					}
				} 
				else
				{	     
					printw("&");
					// In case you want your sprite to be two chars tall
					//mvprintw(relx-1, rely, "");
				}
			}
		}
		move(r-1, 0);

		//std::this_thread::sleep_for(std::chrono::seconds(1));

		// Move
		if ((ch = getch()) < 65 || ch > 68 ) {continue;}
		
		if (ch == KEY_LEFT && this->map[x][y-1] != 9) {y--; food--;}
		else if (ch == KEY_RIGHT && this->map[x][y+1] != 9) {y++; food--;}
		else if (ch == KEY_UP && this->map[x-1][y] != 9) {x--; food--;}
		else if (ch == KEY_DOWN && this->map[x+1][y] != 9) {x++; food--;}

		// Update clear point
		if (this->map[f_x][f_y-1] == 3) {this->map[f_x][f_y] = 0; this->map[f_x][f_y-1] = 2; f_y--;} 
		else if (this->map[f_x-1][f_y] == 3) {this->map[f_x][f_y] = 0; this->map[f_x-1][f_y] = 2; f_x--; printw("GOTEM E ");} 
		else if (this->map[f_x][f_y+1] == 3) {this->map[f_x][f_y] = 0; this->map[f_x][f_y+1] = 2; f_y++; printw("GOTEM S ");} 
		else if (this->map[f_x+1][f_y] == 3) {this->map[f_x][f_y] = 0; this->map[f_x+1][f_y] = 2; f_x++; printw("GOTEM W ");}

		nLoop++;

		clear();

	}
	
	clear();

	if (food == 0) 
	{
		mvprintw(r/2, c/2, "You Suck!");
	} else {
		mvprintw(r/2, c/2, "You Win!");
	}

	getch();

	endwin();
}


string
Level::getName() {return this->name;}

const char *
Level::getCName() {return this->name.c_str();}
