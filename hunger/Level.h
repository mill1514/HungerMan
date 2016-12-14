#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

using namespace std;

class Level
{

	private:

		int numFood;
		string name;

	public:

		Level(string filename);
		vector<vector<int>> map;
		int getNumFood();
		void play();
		string getName();
		const char * getCName();
};

#endif
