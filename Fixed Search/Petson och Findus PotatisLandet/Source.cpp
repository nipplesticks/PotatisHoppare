#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>

enum TYPE
{
	DIRT,
	EMPTY,
	POTATO
};

enum DIRECTION
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct TILE
{
	TYPE type;
	int x, y;

	std::string toString() const
	{
		switch (type)
		{
		case POTATO:
			return "X";
		case EMPTY:
			return "O";
		case DIRT:
			return "#";
		default:
			return "%";
		}
	}
};

struct MOVE
{
	int fromX, fromY, toX, toY;
	std::string toString() const
	{
		return "Move: " + 
			std::to_string(fromX + 1) + 
			"," + 
			std::to_string(fromY + 1) + 
			" to " + 
			std::to_string(toX + 1) + 
			"," + 
			std::to_string(toY + 1) + 
			"\n";
	}
	std::string ID() const
	{
		return std::to_string(fromX) +
			std::to_string(fromY) +
			std::to_string(toX) +
			std::to_string(toY);
	}
};

struct ILLIGAL_MOVE
{
	std::string ID = "";
	bool operator == (const std::string &moveID)
	{
		return ID == moveID;
	}
	void operator = (const std::string &moveID)
	{
		ID = moveID;
	}
	void operator = (const ILLIGAL_MOVE &other)
	{
		ID = other.ID;
	}
};

// globals vars
int gMapWidth = 0;
int gMapHeight = 0;
std::vector<MOVE> gStack;
std::vector<std::vector<MOVE>> gPossibleMoves;
std::vector<ILLIGAL_MOVE> gIlligalMoves;
bool gReversed = false;

TILE** gMap = nullptr;
int gPotatoesLeft = 0;
std::string gCurrentMoveID = "";
long long gCounter = 0;
int gRecord = 32;
bool gFirst = true;

auto gRng = std::default_random_engine{};

void loadMap()
{
	std::ifstream input;
	input.open("map.txt");
	std::string currentLine = "";
	std::string type = "";

	for (int i = 0; i < 2; i++)
	{
		std::getline(input, currentLine);
		std::istringstream s(currentLine);
		s >> type;
		if (type == "HEIGHT") s >> gMapHeight;
		else s >> gMapWidth;
	}
	
	gMap = new TILE*[gMapHeight];

	for (int y = 0; y < gMapHeight; y++)
	{
		gMap[y] = new TILE[gMapWidth];
		std::getline(input, currentLine);
		std::istringstream s(currentLine);
		for (int x = 0; x < gMapWidth; x++)
		{
			TILE t;
			t.x = x;
			t.y = y;
			s >> type;
			if (type == "#")
			{
				t.type = DIRT;
			}
			else if (type == "O")
			{
				t.type = EMPTY;
			}
			else if (type == "X")
			{
				gPotatoesLeft++;
				t.type = POTATO;
			}
			gMap[y][x] = t;
		}
	}
	input.close();
}

void canMoveUp(TILE t, std::vector<MOVE> &p)
{
	if (t.type == POTATO && t.y > 1) // If can potentially move up
	{
		if (gMap[t.y - 1][t.x].type == POTATO && gMap[t.y - 2][t.x].type == EMPTY)
		{
			MOVE m;
			m.fromX = t.x;
			m.fromY = t.y;
			m.toX = t.x;
			m.toY = t.y - 2;

			std::string id = gCurrentMoveID + m.ID();
			bool okToMove = true;
			for (size_t i = 0; i < gIlligalMoves.size() && okToMove; i++)
			{
				if (gIlligalMoves[i].ID == id)
					okToMove = false;
			}
			if (okToMove)
				p.push_back(m);
		}
	}
}

void canMoveRight(TILE t, std::vector<MOVE> &p)
{
	if (t.type == POTATO && t.x < gMapWidth - 2) // If can potentially move right
	{
		if (gMap[t.y][t.x + 1].type == POTATO && gMap[t.y][t.x + 2].type == EMPTY)
		{
			MOVE m;
			m.fromX = t.x;
			m.fromY = t.y;
			m.toX = t.x + 2;
			m.toY = t.y;
			std::string id = gCurrentMoveID + m.ID();
			bool okToMove = true;
			for (size_t i = 0; i < gIlligalMoves.size() && okToMove; i++)
			{
				if (gIlligalMoves[i].ID == id)
					okToMove = false;
			}
			if (okToMove)
				p.push_back(m);
		}
	}
}

void canMoveDown(TILE t, std::vector<MOVE> &p)
{
	if (t.type == POTATO && t.y < gMapHeight - 2) // If can potentially move up
	{
		if (gMap[t.y + 1][t.x].type == POTATO && gMap[t.y + 2][t.x].type == EMPTY)
		{
			MOVE m;
			m.fromX = t.x;
			m.fromY = t.y;
			m.toX = t.x;
			m.toY = t.y + 2;
			std::string id = gCurrentMoveID + m.ID();
			bool okToMove = true;
			for (size_t i = 0; i < gIlligalMoves.size() && okToMove; i++)
			{
				if (gIlligalMoves[i].ID == id)
					okToMove = false;
			}
			if (okToMove)
				p.push_back(m);
		}
	}
}

void canMoveLeft(TILE t, std::vector<MOVE> &p)
{
	if (t.type == POTATO && t.x > 1) // If can potentially move right
	{
		if (gMap[t.y][t.x - 1].type == POTATO && gMap[t.y][t.x - 2].type == EMPTY)
		{
			MOVE m;
			m.fromX = t.x;
			m.fromY = t.y;
			m.toX = t.x - 2;
			m.toY = t.y;
			std::string id = gCurrentMoveID + m.ID();
			bool okToMove = true;
			for (size_t i = 0; i < gIlligalMoves.size() && okToMove; i++)
			{
				if (gIlligalMoves[i].ID == id)
					okToMove = false;
			}
			if (okToMove)
				p.push_back(m);
		}
	}
}

void collectPossibleMoves()
{
	std::vector<MOVE> pm;
	for (int y = 0; y < gMapHeight; y++)
	{
		for (int x = 0; x < gMapWidth; x++)
		{
			canMoveUp(gMap[y][x], pm);
			canMoveRight(gMap[y][x], pm);
			canMoveDown(gMap[y][x], pm);
			canMoveLeft(gMap[y][x], pm);
		}
	}

	std::shuffle(pm.begin(), pm.end(), gRng);

	gPossibleMoves.push_back(pm);
}

void move(MOVE m)
{
	gMap[m.fromY][m.fromX].type = EMPTY;
	gMap[m.toY][m.toX].type = POTATO;
	int middleX, middleY;

	if (m.fromY == m.toY)
	{
		middleX = (m.fromX + m.toX) / 2;
		middleY = m.fromY;
	}
	else
	{
		middleX = m.fromX;
		middleY = (m.fromY + m.toY) / 2;
	}
	gMap[middleY][middleX].type = EMPTY;
	gPotatoesLeft--;

	gCurrentMoveID += m.ID();
}

void reverse(MOVE m)
{
	ILLIGAL_MOVE im;
	im.ID = gCurrentMoveID;
	gIlligalMoves.push_back(im);

	size_t moveSize = m.ID().length();
	size_t idSize = gCurrentMoveID.length();
	gCurrentMoveID.erase(idSize - moveSize, idSize);

	gMap[m.fromY][m.fromX].type = POTATO;
	gMap[m.toY][m.toX].type = EMPTY;
	int middleX, middleY;

	if (m.fromY == m.toY)
	{
		middleX = (m.fromX + m.toX) / 2;
		middleY = m.fromY;
	}
	else
	{
		middleX = m.fromX;
		middleY = (m.fromY + m.toY) / 2;
	}
	gMap[middleY][middleX].type = POTATO;
	gPotatoesLeft++;
}

void printSuccess()
{
	std::ofstream out;
	out.open("How_To_Win.txt");
	for (size_t i = 0; i < gStack.size(); i++)
	{
		out << gStack[i].toString();
	}
	out.close();
}

void printMap()
{
	for (int i = 0; i < gMapHeight; i++)
	{
		for (int k = 0; k < gMapHeight; k++)
		{
			std::cout << gMap[i][k].toString() << " ";
		}
		std::cout << std::endl;
	}
}

void restart()
{
	for (int i = 0; i < gMapHeight; i++)
	{
		delete[] gMap[i];
	}
	delete[] gMap;
	gPotatoesLeft = 0;
	loadMap();
	gStack.clear();
	gPossibleMoves.clear();
	gIlligalMoves.clear();
	gReversed = false;
	gCurrentMoveID = "";
	gCounter = 0;
	gRecord = 32;
	gFirst = true;
}

void play()
{
	bool won = false;
	loadMap();
	std::cout << "Trying to find good start...\n";
	while (!won)
	{
		//printMap();
		if (!gReversed)
			collectPossibleMoves();

		size_t moves = gStack.size();
		//std::cout << "Move nr: " << moves + 1 << std::endl;
		size_t nrOfPossibleMoves = gPossibleMoves[moves].size();
		//std::cout << "Possible moves: " << nrOfPossibleMoves << std::endl;
		//std::cout << "Current moveID: " << gCurrentMoveID << std::endl;

		if (gPotatoesLeft == 1)
		{
			won = true;
			std::cout << "You won!\n";
		}
		else if (nrOfPossibleMoves == 0)
		{
			if (gFirst && gPotatoesLeft > 2)
			{
				restart();
			}
			else if (gFirst)
			{
				gFirst = false;
				std::cout << "Start found!\n";
			}

			if (!gFirst)
			{

				if (gPotatoesLeft < gRecord) gRecord = gPotatoesLeft;

				//std::cout << "Out of moves, Potatoes left: " << gPotatoesLeft << std::endl;
				gReversed = true;
				MOVE m;
				m = gStack[moves - 1];
				gStack.pop_back();
				gPossibleMoves.pop_back();
				reverse(m);



				printf("\rTR: %llu, R: %d", ++gCounter, gRecord);

				//std::cout << "Times reversed: " << ++gCounter << std::endl;
				//std::cout << "Move nr: " << moves - 1 << std::endl;
			}
		}
		else
		{
			gReversed = false;
			MOVE m;
			m = gPossibleMoves[moves][nrOfPossibleMoves - 1];
			gPossibleMoves[moves].pop_back();
			gStack.push_back(m);
			move(m);
			//std::cout << m.toString() << "Potatoes left: " << gPotatoesLeft << std::endl;
		}
	}
	printSuccess();
	printMap();
}

int main()
{
	play();

	for (int i = 0; i < gMapHeight; i++)
	{
		delete[] gMap[i];
	}
	delete[] gMap;
	system("PAUSE");
}