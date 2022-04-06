#include <iostream>
#include <random>
#include <list>
#include <vector>
#include <queue>
#include <limits>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <string>

#define WIDTH 70
#define HEIGHT 70
#define WALL_RATE 47
#define SMOOTHING_COUNT 5


using namespace std;

class MapObject;
string slime = "　　　　　▩▩▩▩▩▩　　　　　　　\n　　　　▩　　　　　▩▩　　　　\n　　　▩　　　　　　　▩▩　　\n　　▩　　　　　　　　▩▩▩　\n　▩　　　　　　■　■　▩▩▩\n　▩　　　　　　■■　■▩▩　\n▩　　　　　　　　■　■▩▩▩\n▩　　　　　　　　■　■　▩▩\n▩　　　　　　　　　　　　▩▩\n▩　　　　　　　　　　　　▩▩\n▩▩　　　　　　　　　　　▩▩\n　▩▩▩▩▩▩▩▩▩▩▩▩▩　\n　▩▩▩▩▩　　　▩▩▩▩▩　\n";
string skeleton = "　　　　　　▩▩▩▩▩▩▩　　　　　\n　　　　　▩　　　　　　　▩　　　　\n　　　　▩　　　　　　　　　▩　　　\n　　　▩　　　　　　　　　　　▩　　\n　　　▩　　▩▩　　　▩▩　　▩　　\n　　　　▩　▩▩　　　▩▩　▩　　　\n　　　▩　　　　　▩　　　　　▩　　\n　　　　▩▩▩　　　　　▩▩▩　　　\n　　　　　▩　▩　▩　▩　▩　　　　\n　　　　　▩▩　▩▩▩　▩▩　　　　\n　　　　　▩　　▩　▩　　▩　　　　\n　　　　　　▩　　　　　▩　　　　　\n　　　　　　　▩▩▩▩▩　　　　　　\n";
string demonKing = "";

int Sign(int value) {
	return (value > 0) - (value < 0);
}

class Point {
public:
	int x;
	int y;

	Point() : x(0), y(0) { }
	Point(int x, int y) : x(x), y(y) { }
	Point(const Point& p) : x(p.x), y(p.y) { }
	void operator=(const Point& p) {
		x = p.x;
		y = p.y;
	}
	bool operator==(Point& p) {
		return x == p.x && y == p.y;
	}
	void operator+(const Point& p) {
		x += p.x;
		y += p.y;
	}
	void operator-(const Point& p) {
		x -= p.x;
		y -= p.y;
	}
};

class Tile {
public:
	enum Type {
		Ground,
		Wall,
	};

	Point* position;
	Type eType;
	MapObject* occupier = nullptr;

	Tile() : position(new Point), eType(Type::Ground) { }
	Tile(int x, int y, Type eType) : position(new Point(x, y)), eType(eType) { }
	~Tile() {
		delete position;
	}

	bool IsAvailable() {
		return (eType == Type::Ground) && occupier == nullptr;
	}

	bool operator==(Tile& t) {
		return *position == *t.position;
	}
};

class Map {
	Map() {
		for (int i = 0; i < WIDTH; i++) {
			grid[i] = new Tile * [HEIGHT];
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
		for (int i = 0; i < rooms->size(); i++) {
			delete rooms->at(i);
		}
		delete rooms;
	}
public:
	class Room {
	public:
		list<Tile*> tiles;
		list<Tile*> edgeTiles;
		list<Room*> connectedRooms;
		bool isMainRoom = false;

		int Size() {
			return tiles.size();
		}

		void Connect(Room& other) {
			connectedRooms.push_back(&other);
			other.connectedRooms.push_back(this);
		}
	};

	Tile*** grid = new Tile * *[WIDTH];
	vector<Room*>* rooms = nullptr;

	static Map& GetInstance() {
		static Map _instance;
		return _instance;
	}

	bool IsInMapRange(int x, int y) {
		return x >= 0 && y >= 0 && x < WIDTH&& y < HEIGHT;
	}

	int GetManhattanDistance(Point& a, Point& b) {
		return abs(a.x - b.x) + abs(a.y - b.y);
	}

	void GenerateMap() {
		MakeNoise();

		for (int i = 0; i < SMOOTHING_COUNT; i++) {
			SmoothingMap();
		}

		rooms = &GetRooms();

		//메인룸 설정
		int biggestRoomSize = INT_MIN;
		int biggestRoomIndex = 0;
		for (int i = 0; i < rooms->size(); i++) {
			int size = rooms->at(i)->Size();
			if (size > biggestRoomSize) {
				biggestRoomSize = size;
				biggestRoomIndex = i;
			}
		}
		rooms->at(biggestRoomIndex)->isMainRoom = true;

		//가장 가까운 방 서로 연결
		ConnectClosestRooms(*rooms);
		//메인룸 연결 안된 방 연결


		//delete rooms
		//
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

	vector<Room*>& GetRooms() {
		vector<Room*>* rooms = new vector<Room*>;
		int** flags = new int* [WIDTH];
		for (int x = 0; x < WIDTH; x++) {
			flags[x] = new int[HEIGHT];
			for (int y = 0; y < HEIGHT; y++) {
				flags[x][y] = 0;
			}
		}

		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				if (grid[x][y]->eType == Tile::Type::Ground) {
					if (flags[x][y] != 1) {
						flags[x][y] = 1;
						Room* room = GetRoom(x, y, flags);
						rooms->push_back(room);
					}
				}
			}
		}
		for (int x = 0; x < WIDTH; x++) {
			delete[] flags[x];
		}
		delete[] flags;

		return *rooms;
	}

	Room* GetRoom(int startX, int startY, int**& flags) {
		Room* room = new Room;

		queue<Tile*> checkNow;
		queue<Tile*> checkNext;

		checkNow.push(grid[startX][startY]);

		while (!checkNow.empty()) {
			Tile* t = checkNow.front();
			checkNow.pop();

			room->tiles.push_back(t);

			int nowX = t->position->x;
			int nowY = t->position->y;

			for (int x = nowX - 1; x <= nowX + 1; x++) {
				for (int y = nowY - 1; y <= nowY + 1; y++) {
					if (IsInMapRange(x, y) && (x == nowX || y == nowY)) {
						if (grid[x][y]->eType == Tile::Type::Ground) {
							if (x == nowX || y == nowY) {
								if (flags[x][y] != 1) {
									flags[x][y] = 1;
									checkNext.push(grid[x][y]);
								}
							}

						}
					}
				}
			}
			if (checkNow.empty()) {
				checkNow.swap(checkNext);
			}
		}

		for (Tile* t : room->tiles) {
			int startX = t->position->x;
			int startY = t->position->y;
			bool isEdge = false;
			for (int x = startX - 1; x <= startX + 1; x++) {
				for (int y = startY - 1; y <= startY + 1; y++) {
					if (IsInMapRange(x, y) && !isEdge) {
						if (grid[x][y]->eType == Tile::Type::Wall) {
							isEdge = true;
							room->edgeTiles.push_back(t);
						}
					}
				}
			}
		}

		return room;
	}

	void ConnectClosestRooms(vector<Room*>& rooms) {
		bool isThereConnectablePoint = false;
		for (Room* r1 : rooms) {
			if (!r1->connectedRooms.empty()) {
				continue;
			}
			isThereConnectablePoint = false;
			int minDistance = INT_MAX;
			Room* closestRoom = nullptr;
			Tile* bestTile1 = nullptr;
			Tile* bestTile2 = nullptr;
			for (Room* r2 : rooms) {
				if (r1 == r2) {
					continue;
				}
				for (auto iter1 = r1->edgeTiles.begin(); iter1 != r1->edgeTiles.end(); iter1++) {
					Tile* t1 = *iter1;
					for (auto iter2 = r2->edgeTiles.begin(); iter2 != r2->edgeTiles.end(); iter2++) {
						Tile* t2 = *iter2;
						int distance = GetManhattanDistance(*t1->position, *t2->position);
						if (distance < minDistance) {
							isThereConnectablePoint = true;
							minDistance = distance;
							bestTile1 = t1;
							bestTile2 = t2;
							closestRoom = r2;
						}
					}
				}
			}
			if (isThereConnectablePoint) {
				MakePassage(bestTile1, bestTile2);
				r1->Connect(*closestRoom);
			}
		}
	}

	void MakePassage(Tile* t1, Tile* t2) {
		list<Point>* line = GetLine(*t1, *t2);
		for (auto iter = line->begin(); iter != line->end(); iter++) {
			MakeCircle(*iter);
		}
		delete line;
	}

	list<Point>*& GetLine(Tile t1, Tile t2) {
		list<Point>* line = new list<Point>;
		Point from = *t1.position;
		Point to = *t2.position;
		int x = from.x;
		int y = from.y;

		int dx = to.x - from.x;
		int dy = to.y - from.y;

		bool inverted = false;
		int step = Sign(dx);
		int gradientStep = Sign(dy);

		int longest = abs(dx);
		int shortest = abs(dy);

		if (longest < shortest) {
			inverted = true;
			longest = abs(dy);
			shortest = abs(dx);

			step = Sign(dy);
			gradientStep = Sign(dx);
		}

		int gradientAccumulation = longest / 2;
		for (int i = 0; i < longest; i++) {
			line->push_back(Point(x,y));

			if (inverted) {
				y += step;
			}
			else {
				x += step;
			}

			gradientAccumulation += shortest;
			if (gradientAccumulation >= longest) {
				if (inverted) {
					x += gradientStep;
				}
				else {
					y += gradientStep;
				}
				gradientAccumulation -= longest;
			}
		}

		return line;
	}

	void MakeCircle(Point p) {
		int startX = p.x;
		int startY = p.y;
		int r = 4;
		for (int x = startX - r; x <= startX + r; x++) {
			for (int y = startY - r; y <= startY + r; y++) {
				if (IsInMapRange(x, y)) {
					if (pow(startX - x, 2) + pow(startY - y, 2) <= pow(r, 2)) {
						grid[x][y]->eType = Tile::Type::Ground;
					}
				}
			}
		}
	}
};

class MapObject {
public:
	enum Type {
		Item,
		Enemy,
		Player
	};
protected:
	string name;
	Point position;
	Type eType = Type::Item;
public:
	string GetName() {
		return name;
	}
	void SetPosition(int x, int y) {
		position = Point(x, y);
	}
	Point GetPosition() {
		return position;
	}
	Type GetType() {
		return eType;
	}
};

class Character : public MapObject {
protected:
	int HP;
	int MP;
	int maxHP;
	int maxMP;
	int Str;
	int Int;
	int Def;
public:
	Character(const char* name, int hp, int mp, int str, int intel, int def) : HP(hp), MP(mp), maxHP(hp), maxMP(mp), Str(str), Int(intel), Def(def) {
		this->name = name;
		eType = Type::Enemy;
	}

	void Damaged(int damage) {
		HP -= damage - Def;
		if (HP < 0) {
			HP = 0;
		}
	}

	void Attack(Character& target) {
		target.Damaged(Str);
	}

	bool IsDead() {
		return HP <= 0;
	}

	int GetHP() {
		return HP;
	}

	int GetMP() {
		return MP;
	}

	int GetMaxHP() {
		return maxHP;
	}

	int GetMaxMP() {
		return maxMP;
	}

	int GetSTR() {
		return Str;
	}

	int GetINT() {
		return Int;
	}

	int GetDEF() {
		return Def;
	}
};

class Player : public Character {
	list<MapObject*> nearObjects;
public:
	Player(const char* name, int hp, int mp, int str, int intel, int def) : Character(name, hp, mp, str, intel, def) {
		eType = Type::Player;
	}

	list<MapObject*>& GetNearObjects() {
		nearObjects.clear();
		for (int x = position.x - 3; x <= position.x + 3; x++) {
			for (int y = position.y - 3; y <= position.y + 3; y++) {
				if (Map::GetInstance().IsInMapRange(x, y) && (x != position.x || y != position.y)) {
					if (Map::GetInstance().grid[x][y]->occupier != nullptr) {
						nearObjects.push_back(Map::GetInstance().grid[x][y]->occupier);
					}
				}
			}
		}
		return nearObjects;
	}
};

class InputManager {
public:
	enum Direction {
		N,
		W,
		E,
		S
	};
private:
	InputManager() { }
	InputManager(InputManager& other) { }
	InputManager& operator=(InputManager& other) { }
	~InputManager() { }

	int GetKey() {
		if (_kbhit()) {
			return _getch();
		}
		return -1;
	}
	void (*MoveEvent)(Direction d) = nullptr;
	void (*SpacebarEvent)() = nullptr;
public:

	static InputManager& GetInstance() {
		static InputManager _instance;
		return _instance;
	}

	void DoSomething() {
		switch (GetKey()) {
		case 'w':
		case 'W':
			if (MoveEvent != nullptr) {
				MoveEvent(Direction::N);
			}
			break;
		case 'a':
		case 'A':
			if (MoveEvent != nullptr) {
				MoveEvent(Direction::W);
			}
			break;
		case 'd':
		case 'D':
			if (MoveEvent != nullptr) {
				MoveEvent(Direction::E);
			}
			break;
		case 's':
		case 'S':
			if (MoveEvent != nullptr) {
				MoveEvent(Direction::S);
			}
			break;
		//spacebar
		case ' ':
			if (SpacebarEvent != nullptr) {
				SpacebarEvent();
			}
			break;
		}
	}

	void SetMoveEvent(void (*event)(Direction d)) {
		MoveEvent = event;
	}

	void SetSpacebarEvent(void (*event)()) {
		SpacebarEvent = event;
	}

	void RemoveMoveEvent() {
		MoveEvent = nullptr;
	}

	void RemoveSpacebarEvent() {
		SpacebarEvent = nullptr;
	}
};

class Camera {
	Camera() {

	}
	Camera(Camera& other) { }
	Camera& operator=(Camera& other) { }
	~Camera() { }
	string screen;
public:
	Point rig = Point(40, 40);

	static Camera& GetInstance() {
		static Camera _instance;
		return _instance;
	}

	void DrawScreen();

	void DrawMonster(string& enemyName) {
		if (enemyName == "슬라임") {
			screen.clear();
			screen = slime;
		}
		else if (enemyName == "스켈레톤") {
			screen.clear();
			screen = skeleton;
		}
		else if (enemyName == "마왕") {
			screen.clear();
			screen = demonKing;
		}
	}
};

class GameManager {
	const int ENEMY_COUNT = 20;
	const int MAP_OBJECT_COUNT = 20;

	GameManager();
	GameManager(GameManager& other) { }
	GameManager& operator=(GameManager& other) { }
	~GameManager() {
		delete player;
		for (int i = 0; i < ENEMY_COUNT; i++) {
			delete enemies[i];
		}
		delete[] enemies;
		for (int i = 0; i < MAP_OBJECT_COUNT; i++) {
			delete mapObjects[i];
		}
		delete[] mapObjects;
	}

	clock_t pastTime = 0;
	clock_t beforeTime = 0;
	clock_t nextTime = 0;

	const double CLOCKS_PER_FRAME = 1.0 / 30.0 * 1000;
	
public:
	Player* player = new Player("플레이어", 100, 50, 20, 15, 5);
	Character* enemy = nullptr;
	Character* enemies[20];
	MapObject* mapObjects[20];

	static GameManager& GetInstance() {
		static GameManager _instance;
		return _instance;
	}

	void InitGame() {
		int x = 0;
		int y = 0;
		while (true) {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
			if (Map::GetInstance().grid[x][y]->IsAvailable()) {
				player->SetPosition(x, y);
				Map::GetInstance().grid[x][y]->occupier = player;
				break;
			}
		}
		Camera::GetInstance().rig.x = x > 16 ? x : 16;
		Camera::GetInstance().rig.x = x < WIDTH - 16 ? x : WIDTH - 16;
		Camera::GetInstance().rig.y = y > 16 ? y : 16;
		Camera::GetInstance().rig.y = y < HEIGHT - 16 ? y : HEIGHT - 16;
		for (int i = 0; i < ENEMY_COUNT / 2; i++) {
			enemies[i] = new Character("슬라임", 30, 0, 5, 0, 1);
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					enemies[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = enemies[i];
					break;
				}
			}
		}
		for (int i = ENEMY_COUNT / 2; i < ENEMY_COUNT - 1; i++) {
			enemies[i] = new Character("스켈레톤", 60, 0, 10, 0, 4);
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					enemies[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = enemies[i];
					break;
				}
			}
		}
		for (int i = ENEMY_COUNT - 1; i < ENEMY_COUNT; i++) {
			enemies[i] = new Character("마왕", 100, 0, 20, 0, 10);
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					enemies[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = enemies[i];
					break;
				}
			}
		}
	}

	void Play() {
		while (true) {
			nextTime = clock();
			pastTime += nextTime - beforeTime;
			beforeTime = nextTime;

			if (pastTime >= CLOCKS_PER_FRAME) {
				pastTime = 0;
				system("cls");

				InputManager::GetInstance().DoSomething();
				Camera::GetInstance().DrawScreen();
			}
		}
	}
};

class State {
protected:
	string name;
	virtual void AddListener() = 0;
	void RemoveListener() {
		InputManager::GetInstance().RemoveMoveEvent();
		InputManager::GetInstance().RemoveSpacebarEvent();
	}
public:
	void OnEnter() {
		AddListener();
	}
	void OnExit() {
		RemoveListener();
	}
	string GetName() {
		return name;
	}
};

class ExploreState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(Moving);
		InputManager::GetInstance().SetSpacebarEvent(Check);
	}
public:
	ExploreState() { 
		name = "ExploreState";
	}
	static void Moving(InputManager::Direction d);
	static void Check();
};

class BattleState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(SelectAction);
		InputManager::GetInstance().SetSpacebarEvent(SpaceBar);
	}
public:
	BattleState() {
		name = "BattleState";
	}
	static void SelectAction(InputManager::Direction d) {

	}
	static void SpaceBar();
};

class AttackingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
	}
public:
	AttackingState() {
		name = "AttackingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
};

class DefensingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
	}
public:
	DefensingState() {
		name = "DefensingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
};

class StateMachine {
	StateMachine() {
		State* es = new ExploreState;
		Transition(*es);
	}
	StateMachine(StateMachine& other) { }
	StateMachine& operator=(StateMachine& other) { }
	~StateMachine() { }

	State* current = nullptr;
public:
	static StateMachine& GetInstance() {
		static StateMachine _instance;
		return _instance;
	}

	void Transition(State& state) {
		if (current != nullptr) {
			current->OnExit();
			delete current;
		}
		current = &state;
		current->OnEnter();
	}

	string GetCurrentStateName() {
		return current->GetName();
	}
};

GameManager::GameManager() {
	Map::GetInstance();
	StateMachine::GetInstance();
	Camera::GetInstance();
}

void Camera::DrawScreen() {
	string nowState = StateMachine::GetInstance().GetCurrentStateName();
	if (nowState == "ExploreState") {
		screen.clear();
		screen += "▨";
		for (int i = 0; i < 30; i++) {
			screen += "▤";
		}
		screen += "▧\n";
		for (int x = rig.x - 15; x < rig.x + 15; x++) {
			screen += "▥";
			for (int y = rig.y - 15; y < rig.y + 15; y++) {
				if (!Map::GetInstance().IsInMapRange(x, y)) {
					continue;
				}
				if (Map::GetInstance().grid[x][y]->occupier != nullptr) {
					string occupierName = Map::GetInstance().grid[x][y]->occupier->GetName();
					if (occupierName == "플레이어") {
						screen += "☆";
					}
					else if (occupierName == "슬라임") {
						screen += "⊙";
					}
					else if (occupierName == "스켈레톤") {
						screen += "＆";
					}
					else if (occupierName == "마왕") {
						screen += "＠";
					}
				}
				else {
					switch (Map::GetInstance().grid[x][y]->eType) {
					case Tile::Type::Ground:
						screen += "□";
						break;
					case Tile::Type::Wall:
						screen += "■";
						break;
					}
				}
			}
			screen += "▥\n";
		}
		screen += "▧";
		for (int i = 0; i < 30; i++) {
			screen += "▤";
		}
		screen += "▨\n";

		screen += "\n";

		Player* p = GameManager::GetInstance().player;
		screen += "[현재 상태] HP: " + to_string(p->GetHP()) + "/" + to_string(p->GetMaxHP());
		screen += " MP: " + to_string(p->GetMP()) + "/" + to_string(p->GetMaxMP());
		screen += " STR: " + to_string(p->GetSTR());
		screen += " INT: " + to_string(p->GetINT());
		screen += " DEF: " + to_string(p->GetDEF());
		screen += "\n";
		screen += "[장비] testA, testB, testC, testD\n";
		screen += "[인벤토리] testA, testB, testC, testD\n";
		screen += "[인접한 것] ";
		list<MapObject*> nearObjects = p->GetNearObjects();
		for (auto iter = nearObjects.begin(); iter != nearObjects.end(); iter++) {
			screen += (*iter)->GetName() + " ";
		}
		cout << screen.c_str() << endl;
	}
	else if (nowState == "BattleState") {
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += "[" + enemyName + "] HP: " + to_string(enemy->GetHP()) + "/" + to_string(enemy->GetMaxHP());
		screen += "\n";
		screen += "SpaceBar: 공격";
		cout << screen.c_str() << endl;
	}
	else if (nowState == "AttackingState") {
		Player* player = GameManager::GetInstance().player;
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += enemyName + "에게 공격을 했다! 피해를 " + to_string(player->GetSTR() - enemy->GetDEF()) + " 줬다.";
		screen += "\n";
		screen += enemyName + "의 HP가 " + to_string(enemy->GetHP()) + "이(가) 되었다.";
		screen += "\n";
		if (enemy->IsDead()) {
			screen += enemyName + "이(가) 쓰러졌다!\n";
		}
		screen += "SpaceBar: 확인";
		cout << screen.c_str() << endl;
	}
	else if (nowState == "DefensingState") {
		Player* player = GameManager::GetInstance().player;
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += enemyName + "의 공격! 피해를 " + to_string(enemy->GetSTR() - player->GetDEF()) + " 받았다.";
		screen += "\n";
		screen += "플레이어의 HP가 " + to_string(player->GetHP()) + "이(가) 되었다.";
		screen += "\n";
		if (player->IsDead()) {
			screen += "패배했다...\n";
		}
		screen += "SpaceBar: 확인";
		cout << screen.c_str() << endl;
	}
}

void ExploreState::Moving(InputManager::Direction d) {
	Point nowPosition = GameManager::GetInstance().player->GetPosition();
	int x = nowPosition.x;
	int y = nowPosition.y;
	switch (d) {
	case InputManager::Direction::N:
		if (!Map::GetInstance().IsInMapRange(x - 1, y)) {
			return;
		}
		if (Map::GetInstance().grid[x - 1][y]->IsAvailable()) {
			GameManager::GetInstance().player->SetPosition(x - 1, y);
			Map::GetInstance().grid[x][y]->occupier = nullptr;
			Map::GetInstance().grid[x - 1][y]->occupier = GameManager::GetInstance().player;
			if (x > 14 && x < WIDTH - 15) {
				Camera::GetInstance().rig.x -= 1;
			}
		}
		break;
	case InputManager::Direction::E:
		if (!Map::GetInstance().IsInMapRange(x, y + 1)) {
			return;
		}
		if (Map::GetInstance().grid[x][y + 1]->IsAvailable()) {
			GameManager::GetInstance().player->SetPosition(x, y + 1);
			Map::GetInstance().grid[x][y]->occupier = nullptr;
			Map::GetInstance().grid[x][y + 1]->occupier = GameManager::GetInstance().player;
			if (y > 15 && y < HEIGHT - 14) {
				Camera::GetInstance().rig.y += 1;
			}
		}
		break;
	case InputManager::Direction::W:
		if (!Map::GetInstance().IsInMapRange(x, y - 1)) {
			return;
		}
		if (Map::GetInstance().grid[x][y - 1]->IsAvailable()) {
			GameManager::GetInstance().player->SetPosition(x, y - 1);
			Map::GetInstance().grid[x][y]->occupier = nullptr;
			Map::GetInstance().grid[x][y - 1]->occupier = GameManager::GetInstance().player;
			if (y > 14 && y < HEIGHT - 15) {
				Camera::GetInstance().rig.y -= 1;
			}
		}
		break;
	case InputManager::Direction::S:
		if (!Map::GetInstance().IsInMapRange(x + 1, y)) {
			return;
		}
		if (Map::GetInstance().grid[x + 1][y]->IsAvailable()) {
			GameManager::GetInstance().player->SetPosition(x + 1, y);
			Map::GetInstance().grid[x][y]->occupier = nullptr;
			Map::GetInstance().grid[x + 1][y]->occupier = GameManager::GetInstance().player;
			if (x > 15 && x < WIDTH - 14) {
				Camera::GetInstance().rig.x += 1;
			}
		}
		break;
	}
}

void ExploreState::Check() {
	list<MapObject*> nearObjects = GameManager::GetInstance().player->GetNearObjects();
	if (nearObjects.empty()) {
		return;
	}
	if (nearObjects.front()->GetType() == MapObject::Type::Enemy) {
		GameManager::GetInstance().enemy = (Character*)nearObjects.front();
		State* bs = new BattleState;
		StateMachine::GetInstance().Transition(*bs);
	}
	else {

	}

}

void BattleState::SpaceBar() {
	Player* player = GameManager::GetInstance().player;
	Character* enemy = GameManager::GetInstance().enemy;
	if (enemy->IsDead()) {

	}
	else if (player->IsDead()){

	}
	else {
		player->Attack(*enemy);

		State* as = new AttackingState;
		StateMachine::GetInstance().Transition(*as);

	}
}

void AttackingState::OK() {
	Player* player = GameManager::GetInstance().player;
	Character* enemy = GameManager::GetInstance().enemy;
	if (enemy->IsDead()) {
		int x = enemy->GetPosition().x;
		int y = enemy->GetPosition().y;
		Map::GetInstance().grid[x][y]->occupier = nullptr;
		enemy = nullptr;
		delete GameManager::GetInstance().enemy;
		GameManager::GetInstance().enemy = nullptr;

		State* es = new ExploreState;
		StateMachine::GetInstance().Transition(*es);
	}
	else {
		enemy->Attack(*player);

		State* ds = new DefensingState;
		StateMachine::GetInstance().Transition(*ds);
	}
}

void DefensingState::OK() {
	Player* player = GameManager::GetInstance().player;
	if (player->IsDead()) {
		//패배스테이트
	}
	else {
		State* bs = new BattleState;
		StateMachine::GetInstance().Transition(*bs);
	}
}

int main() {
	GameManager::GetInstance().InitGame();
	GameManager::GetInstance().Play();

	return 0;
}

