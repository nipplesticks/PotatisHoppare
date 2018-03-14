#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>
const int MAP_WIDTH = 7;
const int MAP_HEIGHT = 7;
const int DIRT = -1;
const int EMPTY = 0;
const int POTATO = 1;

const int UP = 0;
const int DOWN = 1;
const int RIGHT = 2;
const int LEFT = 3;

int gMap[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int gPotatoesLeft = 0;
long long gCounter = 0;
int gRecord = 99999;
std::ofstream gOut;
std::string gWinString = "";


void loadMap()
{
	std::ifstream input;
	input.open("map.txt");
	std::string currentLine = "";
	std::string type = "";

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		std::getline(input, currentLine);
		std::istringstream s(currentLine);
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			s >> type;
			if (type == "X") gMap[y][x] = DIRT;
			else if (type == "O") gMap[y][x] = EMPTY;
			else if (type == "P")
			{
				gMap[y][x] = POTATO;
				gPotatoesLeft++;
			}
		}
	}
	input.close();
}

bool canMoveDown(int y, int x)
{
	if (gMap[y][x] != POTATO || y > MAP_HEIGHT - 3) return false;

	return gMap[y + 1][x] == POTATO && gMap[y + 2][x] == EMPTY;
}

bool canMoveUp(int y, int x)
{
	if (gMap[y][x] != POTATO || y < 2) return false;

	return gMap[y - 1][x] == POTATO && gMap[y - 2][x] == EMPTY;
}

bool canMoveRight(int y, int x)
{
	if (gMap[y][x] != POTATO || x > MAP_WIDTH - 3) return false;

	return gMap[y][x + 1] == POTATO && gMap[y][x + 2] == EMPTY;
}

bool canMoveLeft(int y, int x)
{
	if (gMap[y][x] != POTATO || x < 2) return false;

	return gMap[y][x - 1] == POTATO && gMap[y][x - 2] == EMPTY;
}

bool canMove()
{
	bool canMove = false;
	for (int y = 0; y < MAP_HEIGHT && !canMove; y++)
	{
		for (int x = 0; x < MAP_WIDTH && !canMove; x++)
		{
			canMove = canMoveUp(y, x) || canMoveDown(y, x) || canMoveRight(y, x) || canMoveLeft(y, x);
		}
	}

	return canMove;
}

void moveUp(int y, int x)
{
	gMap[y][x] = EMPTY;
	gMap[y - 1][x] = EMPTY;
	gMap[y - 2][x] = POTATO;

	gWinString += std::to_string(x) + ":" + std::to_string(y) +
		" -->" + std::to_string(x) + ":" + std::to_string(y - 2) + "\n";
}

void moveDown(int y, int x)
{
	gMap[y][x] = EMPTY;
	gMap[y + 1][x] = EMPTY;
	gMap[y + 2][x] = POTATO;

	gWinString += std::to_string(x) + ":" + std::to_string(y) +
		" -->" + std::to_string(x) + ":" + std::to_string(y + 2) + "\n";
}

void moveRight(int y, int x)
{
	gMap[y][x] = EMPTY;
	gMap[y][x + 1] = EMPTY;
	gMap[y][x + 2] = POTATO;

	gWinString += std::to_string(x) + ":" + std::to_string(y) +
		" -->" + std::to_string(x + 2) + ":" + std::to_string(y) + "\n";
}

void moveLeft(int y, int x)
{
	gMap[y][x] = EMPTY;
	gMap[y][x - 1] = EMPTY;
	gMap[y][x - 2] = POTATO;

	gWinString += std::to_string(x) + ":" + std::to_string(y) +
		" -->" + std::to_string(x - 2) + ":" + std::to_string(y) + "\n";
}

bool play()
{
	bool won = false;
	if (canMove())
	{
		bool moved = false;
		while (!moved)
		{
			int y = rand() % MAP_HEIGHT;
			int x = rand() % MAP_WIDTH;
			int dir = rand() % 4;
			moved = true;
			gPotatoesLeft--;
			if (dir == UP && canMoveUp(y, x))
			{
				moveUp(y, x);
			}
			else if (dir == DOWN && canMoveDown(y, x))
			{
				moveDown(y, x);
			}
			else if (dir == RIGHT && canMoveRight(y, x))
			{
				moveRight(y, x);
			}
			else if (dir == LEFT && canMoveLeft(y, x))
			{
				moveLeft(y, x);
			}
			else
			{
				moved = false;
				gPotatoesLeft++;
			}
		}
	}
	else if (gPotatoesLeft == 1)
	{
		won = true;
		gWinString += "Number of Tries: " + std::to_string(++gCounter);
	}
	else
	{
		if (gPotatoesLeft < gRecord) gRecord = gPotatoesLeft;
		printf("\rTries: %lld\tPotatoes Left: %d\tRecord: %d", ++gCounter, gPotatoesLeft, gRecord);
		gWinString = "";
		gPotatoesLeft = 0;
		loadMap();
	}

	return won;
}


int main()
{
	srand(static_cast<unsigned int>(time(0)));
	bool haveWon = false;
	loadMap();
	while (!haveWon)
	{
		haveWon = play();
	}

	std::ofstream out;
	out.open("How_To_Win.txt");
	out << gWinString;
	out.close();
	return 0;
}