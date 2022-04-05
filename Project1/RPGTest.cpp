#include <iostream>
#include <random>
#include <list>
#include <vector>
#include <queue>

#define WIDTH 300
#define HEIGHT 300
#define WALL_RATE 47
#define SMOOTHING_COUNT 4

using namespace std;

class Point {
public:
	int x;
	int y;

	Point() : x(0), y(0) { }
	Point(int x, int y) : x(x), y(y) { }
	Point(Point& p) : x(p.x), y(p.y) { }
	void operator=(Point& p) { 
		x = p.x;
		y = p.y;
	}
	bool operator==(Point& p) {
		return x == p.x && y == p.y;
	}
};

class Tile {
public:
	enum Type {
		Ground,
		Grass,
		Mountain,
		Wall,
	};

	Point* position;
	Type eType;
	void* occupier = NULL;

	Tile() : position(new Point), eType(Type::Ground) { }
	Tile(int x, int y, Type eType) : position(new Point(x,y)), eType(eType) { }
	~Tile() {
		delete position;
	}

	bool IsAvailable() {
		return (eType == Type::Ground || eType == Type::Grass) && occupier == NULL;
	}

	bool operator==(Tile& t) {
		return *position == *t.position;
	}
};

class Map {
	Map() {
		for (int i = 0; i < WIDTH; i++) {
			grid[i] = new Tile*[HEIGHT];
		}
		GenerateMap();
	}
	Map(Map& other) { }
	Map& operator=(Map& other) { }
	~Map() {
		for (int i = 0; i < WIDTH; i++) {
			delete[] grid[i];
		}
		delete[] grid;
	}
public:
	static Map& GetInstance() {
		static Map _instance;
		return _instance;
	}
	
	Tile*** grid = new Tile** [WIDTH];
	vector<list<Tile*>> rooms;

	bool IsInMapRange(int x, int y) {
		return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
	}

	void GenerateMap() {
		MakeNoise();
		
		for (int i = 0; i < SMOOTHING_COUNT; i++) {
			SmoothingMap();
		}

		rooms = GetRooms();

	}

	void MakeNoise() {
		srand(time(NULL));

		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				Tile::Type eType = rand() % 100 < WALL_RATE ? Tile::Type::Wall : Tile::Type::Ground;
				grid[x][y] = new Tile(x, y, eType);
			}
		}
	}

	void SmoothingMap() {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				int surroundingWalls = GetSurroundingWalls(x, y);
				if (surroundingWalls > 4) {
					grid[x][y]->eType = Tile::Type::Wall;
				}
				else if (surroundingWalls < 4) {
					grid[x][y]->eType = Tile::Type::Ground;
				}
			}
		}
	}

	int GetSurroundingWalls(int startX, int startY) {
		int surroundingWallsCount = 0;
		for (int x = startX - 1; x <= startX + 1; x++) {
			for (int y = startY - 1; y <= startY + 1; y++) {
				if (IsInMapRange(x, y)) {
					if (x != startX || y != startY) {
						if (grid[x][y]->eType == Tile::Type::Wall) {
							surroundingWallsCount++;
						}
					}
				}
				else {
					surroundingWallsCount++;
				}
			}
		}
		return surroundingWallsCount;
	}

	vector<list<Tile*>> GetRooms() {

	}

	void DrawGrid() {
		for (int x = 0; x < 30; x++) {
			for (int y = 0; y < 30; y++) {
				if (grid[x][y]->eType == Tile::Type::Ground) {
					cout << "бр";
				}
				else if (grid[x][y]->eType == Tile::Type::Wall) {
					cout << "бс";
				}
			}
			cout << endl;
		}
	}
};


int main() {
	Map::GetInstance().DrawGrid();

	return 0;
}