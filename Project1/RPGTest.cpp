#include <iostream>
#include <random>
#include <list>
#include <vector>
#include <queue>

class Point {
public:
	int x;
	int y;

	Point(int x, int y) : x(x), y(y) { }
};

class Tile {
public:
	enum Type {
		Ground,
		Grass,
		Mountain,
		Wall,
	};

	Point position;
	Type eType;
	void* occupier = NULL;

	Tile(int x, int y, Type eType) : position(new Point(x,y)), eType(eType) { }

	bool IsAvailable() {
		return (eType == Type::Ground || eType == Type::Grass) && occupier == NULL;
	}
};

class Map {
	Map* _instance = NULL;
	Map() { }
public:
	Map& GetInstance() {
		if (_instance == NULL) {
			_instance = new Map();
		}
		return *_instance;
	}
};


void main() {

}