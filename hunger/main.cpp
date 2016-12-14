#include "Level.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{

	string lvlName;
	if (argc == 1) {
		lvlName = "trial.lvl";
	} else {
		lvlName = argv[1];
	}


	Level * lvl = new Level(lvlName);

	lvl->play();

	delete lvl;

	return 0;

}
