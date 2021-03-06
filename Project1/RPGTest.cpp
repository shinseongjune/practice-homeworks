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
#include <algorithm>

#define WIDTH 70
#define HEIGHT 70
#define WALL_RATE 47
#define SMOOTHING_COUNT 5


using namespace std;

class MapObject;
string slime = "　　　　　▩▩▩▩▩▩　　　　　　　\n　　　　▩　　　　　▩▩　　　　\n　　　▩　　　　　　　▩▩　　\n　　▩　　　　　　　　▩▩▩　\n　▩　　　　　　■　■　▩▩▩\n　▩　　　　　　■■　■▩▩　\n▩　　　　　　　　■　■▩▩▩\n▩　　　　　　　　■　■　▩▩\n▩　　　　　　　　　　　　▩▩\n▩　　　　　　　　　　　　▩▩\n▩▩　　　　　　　　　　　▩▩\n　▩▩▩▩▩▩▩▩▩▩▩▩▩　\n　▩▩▩▩▩　　　▩▩▩▩▩　\n";
string skeleton = "　　　　　　▩▩▩▩▩▩▩　　　　　\n　　　　　▩　　　　　　　▩　　　　\n　　　　▩　　　　　　　　　▩　　　\n　　　▩　　　　　　　　　　　▩　　\n　　　▩　　▩▩　　　▩▩　　▩　　\n　　　　▩　▩▩　　　▩▩　▩　　　\n　　　▩　　　　　▩　　　　　▩　　\n　　　　▩▩▩　　　　　▩▩▩　　　\n　　　　　▩　▩　▩　▩　▩　　　　\n　　　　　▩▩　▩▩▩　▩▩　　　　\n　　　　　▩　　▩　▩　　▩　　　　\n　　　　　　▩　　　　　▩　　　　　\n　　　　　　　▩▩▩▩▩　　　　　　\n";
string demonKing = "　　　▩　　　　　　　　　▩　　　\n　　　▩▩　　　　　　　▩▩　　　\n　　　▩▩▩　　　　　▩▩▩　　　\n　　　　▩▩　　　　　▩▩　　　　\n　　　　▩▩▩▩▩▩▩▩▩　　　　\n　　　　▩　　　　　　　▩　　　　\n　　　▩　▩　　　　　▩　▩　　　\n　　　▩　　▩　　　▩　　▩　　　\n　　　▩　　　　　　　　　▩　　　\n　　　▩　　　　▩　　　　▩　　　\n　　　▩　　　▩　▩　　　▩　　　\n　　　　▩　　　　　　　▩　　　　\n　　　　　▩▩▩▩▩▩▩　　　　　\n";
string town = "　　　▩　　　　　　　　　\n　　　▩　　　　　　　　　　　　　　\n　　▩▩▩　　　　　　　　　　　　　\n　　　▩　　　　　　　　　　　　　　\n　　▩▩▩▩▩　　　　　　　　　　　\n　▩　　　▩▩▩▩　　　　　　　　　\n　▩　　　▩　　▩▩　　▩　　　　　\n　▩　　　▩　　▩▩　▩　▩　　　　\n　　▩　　▩▩▩▩　▩　　　▩　　　\n▩▩▩　　　　▩▩▩▩▩▩▩▩▩▩▩\n　　▩▩▩　　　　　　　　　　▩　▩\n　　　　　▩　　　　　　　　　　　▩\n　　　　　▩▩▩▩▩▩▩▩▩▩▩▩▩\n";

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
	~Tile() { }

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
	~Map() { }
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
			for (Room* r : connectedRooms) {
				if (find(r->connectedRooms.begin(), r->connectedRooms.end(), &other) != r->connectedRooms.end()) {
					continue;
				}
				r->Connect(other);
			}
			for (Room* r : other.connectedRooms) {
				if (find(r->connectedRooms.begin(), r->connectedRooms.end(), this) != r->connectedRooms.end()) {
					continue;
				}
				r->Connect(*this);
			}
		}
		bool IsConnectedWithMainRoom() {
			if (isMainRoom) {
				return true;
			}
			for (Room* r : connectedRooms) {
				if (r->isMainRoom) {
					return true;
				}
			}
			return false;
		}
	};

	Tile*** grid = new Tile * *[WIDTH];
	vector<Room*>* rooms = nullptr;
	Room* mainRoom = nullptr;

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
		mainRoom = rooms->at(biggestRoomIndex);

		ConnectClosestRooms(*rooms);
		ConnectMainRoom();
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
			if (flags[x] != nullptr) {
				delete[] flags[x];
			}
		}
		if (flags != nullptr) {
			delete[] flags;
		}

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
		if (line != nullptr) {
			delete line;
		}
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
			line->push_back(Point(x, y));

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

	void ConnectMainRoom() {
		for (int i = 0; i < rooms->size(); i++) {
			Room* room = rooms->at(i);
			if (room->IsConnectedWithMainRoom()) {
				continue;
			}

			bool isThereConnectablePoint = false;
			int minDistance = INT_MAX;
			Tile* bestTile1 = nullptr;
			Tile* bestTile2 = nullptr;

			for (auto iter1 = room->edgeTiles.begin(); iter1 != room->edgeTiles.end(); iter1++) {
				Tile* t1 = *iter1;
				for (auto iter2 = mainRoom->edgeTiles.begin(); iter2 != mainRoom->edgeTiles.end(); iter2++) {
					Tile* t2 = *iter2;
					int distance = GetManhattanDistance(*t1->position, *t2->position);
					if (distance < minDistance) {
						isThereConnectablePoint = true;
						minDistance = distance;
						bestTile1 = t1;
						bestTile2 = t2;
					}
				}
			}
			for (Room* r : room->connectedRooms) {
				for (auto iter1 = r->edgeTiles.begin(); iter1 != r->edgeTiles.end(); iter1++) {
					Tile* t1 = *iter1;
					for (auto iter2 = mainRoom->edgeTiles.begin(); iter2 != mainRoom->edgeTiles.end(); iter2++) {
						Tile* t2 = *iter2;
						int distance = GetManhattanDistance(*t1->position, *t2->position);
						if (distance < minDistance) {
							isThereConnectablePoint = true;
							minDistance = distance;
							bestTile1 = t1;
							bestTile2 = t2;
						}
					}
				}
			}
			if (isThereConnectablePoint) {
				MakePassage(bestTile1, bestTile2);
				room->Connect(*mainRoom);
			}
		}
	}
};

class StatModifier;

class Stat {
	bool isDirty = true;
	float nowValue = 0;
	vector<StatModifier> modifiers;

public:
	enum Type {
		MaxHP,
		MaxMP,
		Strength,
		Intelligence,
		Defense,
		__Count
	};
	float BaseValue = 0;
	float ValuePerLevel = 0;

	Stat(float base = 0, float vpl = 0) : BaseValue(base), ValuePerLevel(vpl) { }

	float GetValue();

	vector<StatModifier> GetModifiersVector() {
		return modifiers;
	}

	void SetModifier(StatModifier mod);

	void RemoveModifiersBySetter(MapObject* setter);

	void LevelUp() {
		BaseValue += ValuePerLevel;
		isDirty = true;
	}
};

class StatModifier {
public:
	enum Type {
		BaseFlat,
		BaseMult,
		TotalFlat,
		TotalMult
	};
	string Name;
	float Value;
	MapObject* Setter = nullptr;
	Type eType;
	Stat::Type statType;

	StatModifier(const char* name, float value, MapObject* setter, Type type, Stat::Type sType) : Name(name), Value(value), Setter(setter), eType(type), statType(sType) { }
	int GetOrder() {
		return (int)eType;
	}
	bool operator==(MapObject* setter) {
		return Setter == setter;
	}
};

float Stat::GetValue() {
	if (isDirty) {
		nowValue = BaseValue;
		for (auto iter = modifiers.begin(); iter != modifiers.end(); iter++) {
			switch ((*iter).eType) {
			case StatModifier::Type::BaseFlat:
			case StatModifier::Type::TotalFlat:
				nowValue += (*iter).Value;
				break;
			case StatModifier::Type::BaseMult:
			case StatModifier::Type::TotalMult:
				nowValue *= (*iter).Value;
				break;
			}
		}
		isDirty = false;
	}
	return nowValue;
}

void Stat::SetModifier(StatModifier mod) {
	string name = mod.Name;
	for (auto iter = modifiers.begin(); iter != modifiers.end(); iter++) {
		if ((*iter).Name == name) {
			return;
		}
	}
	modifiers.push_back(mod);
	sort(modifiers.begin(), modifiers.end(), ([](StatModifier a, StatModifier b) {
		return a.GetOrder() > b.GetOrder();
		}));
	isDirty = true;
}

void Stat::RemoveModifiersBySetter(MapObject* setter) {
	auto iter = remove_if(modifiers.begin(), modifiers.end(), [=](StatModifier mod) {
		return mod == setter;
		});
	modifiers.erase(iter, modifiers.end());
	isDirty = true;
}

class Equippable;
class Character;

class ItemSlot {
public:
	enum Type {
		Head,
		Body,
		Arm,
		Leg,
		Weapon,
		Sub,
		__Count
	};

	Equippable* equipped = nullptr;
	Type eType = Type::Weapon;
};

class MapObject {
public:
	enum Type {
		Equippable,
		HPPotion,
		MPPotion,
		Thrower,
		Enemy,
		Player,
		Town
	};
protected:
	string name;
	Point position;
	Type eType = Type::Equippable;
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

class Equippable : public MapObject {
public:
	struct ItemOption {
		Stat::Type statType;
		StatModifier::Type modType;
		float value;
		ItemOption(Stat::Type statType, StatModifier::Type modType, float value) : statType(statType), modType(modType), value(value) { }
	};

	ItemSlot::Type slotType;
	vector<ItemOption> options;
	int cost;

	Equippable(const char* name, ItemSlot::Type slotType, int cost) : slotType(slotType), cost(cost) {
		this->name = name;
		eType = Type::Equippable;
	}

	void AddOption(ItemOption option) {
		options.push_back(option);
	}
};

class Consumable : public MapObject {
public:
	int value;
	int cost;

	Consumable(const char* name, Type type, int value, int cost) : value(value), cost(cost) {
		this->name = name;
		eType = type;
	}

};

class Character : public MapObject {
protected:
	float HP;
	float MP;

	int Level;
	int EXP;
	int requiredEXP;
public:
	Stat stats[Stat::Type::__Count];
	ItemSlot slots[ItemSlot::Type::__Count];
	vector<MapObject*> inventory;
	int gold;

	Character(const char* name, float hp, float mp, float str, float intel, float def, int gold = 0, float hppl = 0, float mppl = 0, float strpl = 0, float intpl = 0, float defpl = 0) : HP(hp), MP(mp), gold(gold) {
		this->name = name;
		stats[Stat::Type::MaxHP].BaseValue = hp;
		stats[Stat::Type::MaxHP].ValuePerLevel = hppl;
		stats[Stat::Type::MaxMP].BaseValue = mp;
		stats[Stat::Type::MaxMP].ValuePerLevel = mppl;
		stats[Stat::Type::Strength].BaseValue = str;
		stats[Stat::Type::Strength].ValuePerLevel = strpl;
		stats[Stat::Type::Intelligence].BaseValue = intel;
		stats[Stat::Type::Intelligence].ValuePerLevel = intpl;
		stats[Stat::Type::Defense].BaseValue = def;
		stats[Stat::Type::Defense].ValuePerLevel = defpl;

		for (int i = 0; i < ItemSlot::Type::__Count; i++) {
			slots[i].eType = (ItemSlot::Type)i;
		}

		eType = Type::Enemy;
		Level = 1;
		EXP = 0;
		requiredEXP = Level * 50;
	}

	void Damaged(float damage) {
		HP -= max(damage - stats[Stat::Type::Defense].GetValue(), 0);
		if (HP < 0) {
			HP = 0;
		}
	}

	void Healed(float heal) {
		HP = min(HP + heal, stats[Stat::Type::MaxHP].GetValue());
	}

	void Attack(Character& target) {
		target.Damaged(stats[Stat::Type::Strength].GetValue());
	}

	bool IsDead() {
		return HP <= 0;
	}

	float GetHP() {
		return HP;
	}

	void SetHP(float hp) {
		HP = hp;
		if (HP > GetMaxHP()) {
			HP = GetMaxHP();
		}
	}

	float GetMP() {
		return MP;
	}

	void SetMP(float mp) {
		MP = mp;
		if (MP > GetMaxMP()) {
			MP = GetMaxMP();
		}
	}

	float GetMaxHP() {
		return stats[Stat::Type::MaxHP].GetValue();
	}

	float GetMaxMP() {
		return stats[Stat::Type::MaxMP].GetValue();
	}

	float GetSTR() {
		return stats[Stat::Type::Strength].GetValue();
	}

	float GetINT() {
		return stats[Stat::Type::Intelligence].GetValue();
	}

	float GetDEF() {
		return stats[Stat::Type::Defense].GetValue();
	}

	int GetLVL() {
		return Level;
	}

	int GetEXP() {
		return EXP;
	}

	void GainEXP(int exp) {
		EXP += exp;
		while (EXP >= requiredEXP) {
			int remaining = EXP - requiredEXP;
			LVLUp();
			EXP = remaining;
		}
	}

	void LVLUp() {
		Level++;
		stats[Stat::Type::MaxHP].LevelUp();
		stats[Stat::Type::MaxMP].LevelUp();
		HP = stats[Stat::Type::MaxHP].GetValue();
		MP = stats[Stat::Type::MaxMP].GetValue();
		stats[Stat::Type::Strength].LevelUp();
		stats[Stat::Type::Intelligence].LevelUp();
		stats[Stat::Type::Defense].LevelUp();
		EXP = 0;
		requiredEXP = Level * 50;
	}

	void Unequip(ItemSlot::Type type) {
		if (slots[type].equipped == nullptr) {
			return;
		}
		::Equippable* item = slots[type].equipped;

		vector<Equippable::ItemOption> options = item->options;

		for (int i = 0; i < options.size(); i++) {
			Stat::Type statType = options[i].statType;
			stats[statType].RemoveModifiersBySetter(item);
		}

		if (HP > GetMaxHP()) {
			HP = GetMaxHP();
		}
		if (MP > GetMaxMP()) {
			MP = GetMaxMP();
		}

		inventory.push_back(item);
		slots[type].equipped = nullptr;
	}

	void Equip(::Equippable* item) {
		ItemSlot::Type slotType = item->slotType;
		if (slots[slotType].equipped != nullptr) {
			Unequip(slotType);
		}

		vector<Equippable::ItemOption> options = item->options;
		for (int i = 0; i < options.size(); i++) {
			Equippable::ItemOption option = options[i];
			StatModifier mod((item->GetName().c_str()), option.value, item, option.modType, option.statType);
			stats[option.statType].SetModifier(mod);
		}

		if (HP > GetMaxHP()) {
			HP = GetMaxHP();
		}
		if (MP > GetMaxMP()) {
			MP = GetMaxMP();
		}

		slots[slotType].equipped = item;
		RemoveItem(item);
	}

	void GetItem(MapObject* item) {
		inventory.push_back(item);
	}

	void RemoveItem(MapObject* item) {
		for (auto iter = inventory.begin(); iter != inventory.end(); iter++) {
			if (*iter == item) {
				inventory.erase(iter);
				break;
			}
		}
	}
	void UseItem(int i) {
		if (i < 0 || i >= inventory.size()) {
			return;
		}
		if (inventory[i]->GetType() == MapObject::Type::Equippable) {
			Equip((::Equippable*)inventory[i]);
		}
		else if (inventory[i]->GetType() == MapObject::Type::HPPotion) {
			Consumable* item = (Consumable*)inventory[i];
			Healed(item->value);
			RemoveItem(item);
		}
		else if (inventory[i]->GetType() == MapObject::Type::MPPotion) {
			Consumable* item = (Consumable*)inventory[i];
			MP += item->value;
			if (MP > GetMaxMP()) {
				MP = GetMaxMP();
			}
			RemoveItem(item);
		}
	}
};

class Player : public Character {
	list<MapObject*> nearObjects;
public:
	Player(const char* name, float hp, float mp, float str, float intel, float def, int gold, float hppl, float mppl, float strpl, float intpl, float defpl) : Character(name, hp, mp, str, intel, def, gold, hppl, mppl, strpl, intpl, defpl) {
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

	void Buy(::Equippable* item) {
		if (item->cost > gold) {
			return;
		}
		else {
			gold -= item->cost;
			inventory.push_back(item);
		}
	}

	void Sell(int i) {
		if (i < 0 || i >= inventory.size()) {
			return;
		}
		if (inventory[i]->GetType() == MapObject::Type::Equippable) {
			gold += ((::Equippable*)inventory[i])->cost;
		}
		else {
			gold += ((::Consumable*)inventory[i])->cost;
		}
		auto iter = inventory.begin() + i;
		inventory.erase(iter);
	}

	void GainGold(int gold) {
		this->gold += gold;
	}
};

class Town : public MapObject {
public:
	Town(const char* name) {
		this->name = name;
		eType = Type::Town;
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
	void (*NumberEvent)(int i) = nullptr;
public:

	static InputManager& GetInstance() {
		static InputManager _instance;
		return _instance;
	}

	void DoSomething() {
		int downKey = GetKey();
		switch (downKey) {
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
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (NumberEvent != nullptr) {
				NumberEvent(downKey - '0');
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

	void SetNumberEvent(void (*event)(int)) {
		NumberEvent = event;
	}

	void RemoveMoveEvent() {
		MoveEvent = nullptr;
	}

	void RemoveSpacebarEvent() {
		SpacebarEvent = nullptr;
	}

	void RemoveNumberEvent() {
		NumberEvent = nullptr;
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

	string DrawScreen();

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
	const int MAP_OBJECT_COUNT = 10;

	GameManager();
	GameManager(GameManager& other) { }
	GameManager& operator=(GameManager& other) { }
	~GameManager() { }

	clock_t pastTime = 0;
	clock_t beforeTime = 0;
	clock_t nextTime = 0;

	const double CLOCKS_PER_FRAME = 1.0 / 15.0 * 1000;

	string screenBuffer;

public:
	Player* player = new Player("플레이어", 100, 50, 20, 15, 5, 1000, 10, 10, 5, 5, 5);
	Character* enemy = nullptr;
	Character* enemies[20];
	MapObject* mapObjects[20];
	Town* towns[3];
	Town* town = nullptr;
	bool IsGameOver = false;

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
		Camera::GetInstance().rig.x = x <= 15 ? 16 : x >= WIDTH - 15 ? WIDTH - 15 : x;
		Camera::GetInstance().rig.y = y <= 15 ? 16 : y >= HEIGHT - 15 ? HEIGHT - 15 : y;

		towns[0] = new Town("개구리 마을");
		bool isThereTown = false;
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (Map::GetInstance().grid[i][j]->IsAvailable()) {
					towns[0]->SetPosition(i, j);
					Map::GetInstance().grid[i][j]->occupier = towns[0];
					isThereTown = true;
					break;
				}
			}
			if (isThereTown) break;
		}
		towns[1] = new Town("뉴욕");
		while (true) {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
			if (Map::GetInstance().grid[x][y]->IsAvailable()) {
				towns[1]->SetPosition(x, y);
				Map::GetInstance().grid[x][y]->occupier = towns[1];
				break;
			}
		}
		towns[2] = new Town("행복 마을");
		while (true) {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
			if (Map::GetInstance().grid[x][y]->IsAvailable()) {
				towns[2]->SetPosition(x, y);
				Map::GetInstance().grid[x][y]->occupier = towns[2];
				break;
			}
		}

		for (int i = 0; i < ENEMY_COUNT / 2; i++) {
			enemies[i] = new Character("슬라임", 30, 0, 5, 0, 1, 30);
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
			enemies[i] = new Character("스켈레톤", 60, 0, 15, 0, 4, 70);
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
			enemies[i] = new Character("마왕", 800, 0, 40, 0, 18);
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

		for (int i = 0; i < 1; i++) {
			Equippable::ItemOption option = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::BaseFlat, 10);
			Equippable* e = new Equippable("대검", ItemSlot::Type::Weapon, 100);
			e->AddOption(option);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 1; i < 3; i++) {
			Equippable::ItemOption option = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::BaseFlat, 3);
			Equippable* e = new Equippable("나무 갑옷", ItemSlot::Type::Body, 50);
			e->AddOption(option);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 3; i < 4; i++) {
			Consumable* e = new Consumable("HP포션", MapObject::Type::HPPotion, 30, 150);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 3; i < 4; i++) {
			Consumable* e = new Consumable("MP포션", MapObject::Type::MPPotion, 10, 150);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 5; i < 7; i++) {
			Equippable::ItemOption option = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::TotalMult, 1.6);
			Equippable* e = new Equippable("힘의 장갑", ItemSlot::Type::Arm, 30);
			e->AddOption(option);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 7; i < 10; i++) {
			Equippable::ItemOption option1 = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::BaseMult, 2);
			Equippable::ItemOption option2 = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::BaseMult, 0.5);
			Equippable* e = new Equippable("광전사의 투구", ItemSlot::Type::Head, 100);
			e->AddOption(option1);
			e->AddOption(option2);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 10; i < 12; i++) {
			Equippable::ItemOption option = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::TotalMult, 1.8);
			Equippable* e = new Equippable("강철 장화", ItemSlot::Type::Leg, 20);
			e->AddOption(option);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 12; i < 14; i++) {
			Consumable* e = new Consumable("짱돌", MapObject::Type::Thrower, 30, 60);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 14; i < 17; i++) {
			Consumable* e = new Consumable("폭탄", MapObject::Type::Thrower, 70, 100);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 17; i < 19; i++) {
			Equippable::ItemOption option = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::TotalFlat, 5);
			Equippable* e = new Equippable("나무 방패", ItemSlot::Type::Sub, 30);
			e->AddOption(option);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}

		for (int i = 19; i < 20; i++) {
			Equippable::ItemOption option1 = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::BaseFlat, 6);
			Equippable::ItemOption option2 = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::BaseFlat, 6);
			Equippable* e = new Equippable("쌍절곤", ItemSlot::Type::Weapon, 20);
			e->AddOption(option1);
			e->AddOption(option2);
			mapObjects[i] = e;
			while (true) {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
				if (Map::GetInstance().grid[x][y]->IsAvailable()) {
					mapObjects[i]->SetPosition(x, y);
					Map::GetInstance().grid[x][y]->occupier = mapObjects[i];
					break;
				}
			}
		}
	}

	void Play() {
		while (true) {
			if (IsGameOver) {
				break;
			}
			nextTime = clock();
			pastTime += nextTime - beforeTime;
			beforeTime = nextTime;

			if (pastTime >= CLOCKS_PER_FRAME) {
				pastTime = 0;

				InputManager::GetInstance().DoSomething();
				string screen = Camera::GetInstance().DrawScreen();
				if (screen != screenBuffer) {
					system("cls");
					cout << screen.c_str() << endl;
					screenBuffer = screen;
				}
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
		InputManager::GetInstance().RemoveNumberEvent();
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
		InputManager::GetInstance().SetNumberEvent(UseItem);
	}
public:
	ExploreState() {
		name = "ExploreState";
	}
	static void Moving(InputManager::Direction d);
	static void Check();
	static void UseItem(int i) {
		Player* player = GameManager::GetInstance().player;
		if (i == 8) {
			for (int i = 0; i < ItemSlot::Type::__Count; i++) {
				player->Unequip((ItemSlot::Type)i);
			}
			return;
		}
		if (player->inventory.size() > 8 && i == 9) {
			if (player->inventory.size() > 8) {
				for (int i = 0; i < 8; i++) {
					MapObject* item = player->inventory.back();
					player->inventory.insert(player->inventory.begin(), item);
					player->inventory.pop_back();
				}
			}
		}
		else if (i >= 0 || i < 8) {
			GameManager::GetInstance().player->UseItem(i);
		}
	}
};

class BattleState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(SelectAction);
		InputManager::GetInstance().SetSpacebarEvent(SpaceBarEvent);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	BattleState() {
		name = "BattleState";
	}
	static void SelectAction(InputManager::Direction d) {

	}
	static void SpaceBarEvent();
	static void NumberEvent(int i);
};

class AttackingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	AttackingState() {
		name = "AttackingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NumberEvent(int i) {

	}
};

class DefensingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	DefensingState() {
		name = "DefensingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NumberEvent(int i) {

	}
};

class GameOverState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	GameOverState() {
		name = "GameOverState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NumberEvent(int i) {

	}
};

class TownState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	TownState() {
		name = "TownState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NumberEvent(int i);
};

class BuyingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	BuyingState() {
		name = "BuyingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NumberEvent(int i);
};

class SellingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NumberEvent);
	}
public:
	SellingState() {
		name = "SellingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NumberEvent(int i);
};

class WinState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NoAction);
	}
public:
	WinState() {
		name = "WinState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK() {
		GameManager::GetInstance().IsGameOver = true;
	}
	static void NoAction(int i) {

	}
};

class ThrowingState : public State {
protected:
	void AddListener() {
		InputManager::GetInstance().SetMoveEvent(NoAction);
		InputManager::GetInstance().SetSpacebarEvent(OK);
		InputManager::GetInstance().SetNumberEvent(NoAction);
	}
public:
	ThrowingState() {
		name = "ThrowingState";
	}
	static void NoAction(InputManager::Direction d) {

	}
	static void OK();
	static void NoAction(int i) {

	}
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

string Camera::DrawScreen() {
	string nowState = StateMachine::GetInstance().GetCurrentStateName();
	if (nowState == "ExploreState") {
		screen.clear();
		screen += "▨";
		for (int i = 0; i < 31; i++) {
			screen += "▤";
		}
		screen += "▧\n";
		for (int x = rig.x - 16; x < rig.x + 15; x++) {
			screen += "▥";
			for (int y = rig.y - 16; y < rig.y + 15; y++) {
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
					else if (occupierName == "대검") {
						screen += "†";
					}
					else if (occupierName == "나무 갑옷") {
						screen += "♧";
					}
					else if (occupierName == "힘의 장갑") {
						screen += "＊";
					}
					else if (occupierName == "광전사의 투구") {
						screen += "◇";
					}
					else if (occupierName == "강철 장화") {
						screen += "↔";
					}
					else if (occupierName == "나무 방패") {
						screen += "®";
					}
					else if (occupierName == "쌍절곤") {
						screen += "‡";
					}
					else if (occupierName == "짱돌") {
						screen += "▣";
					}
					else if (occupierName == "폭탄") {
						screen += "♨";
					}
					else if (occupierName == "HP포션") {
						screen += "△";
					}
					else if (occupierName == "MP포션") {
						screen += "▽";
					}
					else {
						screen += "◎";
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
		for (int i = 0; i < 31; i++) {
			screen += "▤";
		}
		screen += "▨\n";

		screen += "\n";

		Player* p = GameManager::GetInstance().player;
		screen += "[현재 상태](Lv." + to_string((int)p->GetLVL()) + ") HP: " + to_string((int)p->GetHP()) + "/" + to_string((int)p->GetMaxHP());
		screen += " MP: " + to_string((int)p->GetMP()) + "/" + to_string((int)p->GetMaxMP());
		screen += " STR: " + to_string((int)p->GetSTR());
		screen += " INT: " + to_string((int)p->GetINT());
		screen += " DEF: " + to_string((int)p->GetDEF());
		screen += " [Gold]: " + to_string(p->gold) + "G";
		screen += "\n";

		screen += "[장비]";
		screen += " 무기 : ";
		screen += p->slots[ItemSlot::Type::Weapon].equipped == nullptr ? "없음" : p->slots[ItemSlot::Type::Weapon].equipped->GetName();
		screen += " / 보조장비 : ";
		screen += p->slots[ItemSlot::Type::Sub].equipped == nullptr ? "없음" : p->slots[ItemSlot::Type::Sub].equipped->GetName();
		screen += " / 머리 : ";
		screen += p->slots[ItemSlot::Type::Head].equipped == nullptr ? "없음" : p->slots[ItemSlot::Type::Head].equipped->GetName();
		screen += " / 몸 : ";
		screen += p->slots[ItemSlot::Type::Body].equipped == nullptr ? "없음" : p->slots[ItemSlot::Type::Body].equipped->GetName();
		screen += " / 팔 : ";
		screen += p->slots[ItemSlot::Type::Arm].equipped == nullptr ? "없음" : p->slots[ItemSlot::Type::Arm].equipped->GetName();
		screen += " / 다리 : ";
		screen += p->slots[ItemSlot::Type::Leg].equipped == nullptr ? "없음" : p->slots[ItemSlot::Type::Leg].equipped->GetName();
		screen += "\n";

		screen += "(8) : 모든 장비 해제";
		screen += "\n";

		screen += "[인벤토리] ";
		screen += "\n";
		for (int i = 0; i < min(p->inventory.size(), 8); i++) {
			screen += "(" + to_string(i) + ") : " + p->inventory[i]->GetName() + " ";
		}
		if (p->inventory.size() > 8) {
			screen += "(9) : >>다음 아이템>>";
		}
		screen += "\n";
		screen += "[인접한 것] ";
		list<MapObject*> nearObjects = p->GetNearObjects();
		for (auto iter = nearObjects.begin(); iter != nearObjects.end(); iter++) {
			screen += (*iter)->GetName() + " ";
		}
	}
	else if (nowState == "BattleState") {
		Player* player = GameManager::GetInstance().player;
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += "[" + enemyName + "] HP: " + to_string((int)enemy->GetHP()) + "/" + to_string((int)enemy->GetMaxHP());
		screen += "\n";
		screen += "SpaceBar: 공격";
		screen += "\n";

		bool isThereStone = false;
		bool isThereBomb = false;
		for (auto iter = player->inventory.begin(); iter != player->inventory.end(); iter++) {
			if ((*iter)->GetType() == MapObject::Type::Thrower) {
				if ((*iter)->GetName() == "짱돌") {
					isThereStone = true;
				}
				else if ((*iter)->GetName() == "폭탄") {
					isThereBomb = true;
				}
			}
		}
		if (isThereStone) {
			screen += "(0) : 짱돌 ";
		}
		if (isThereBomb) {
			screen += "(1) : 폭탄 ";
		}
	}
	else if (nowState == "AttackingState") {
		Player* player = GameManager::GetInstance().player;
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += enemyName + "에게 공격을 했다! 피해를 " + to_string(max(0, (int)(player->GetSTR() - enemy->GetDEF()))) + " 줬다.";
		screen += "\n";
		screen += enemyName + "의 HP가 " + to_string((int)enemy->GetHP()) + "이(가) 되었다.";
		screen += "\n";
		if (enemy->IsDead()) {
			screen += enemyName + "이(가) 쓰러졌다!\n";
		}
		screen += "SpaceBar: 확인";
	}
	else if (nowState == "DefensingState") {
		Player* player = GameManager::GetInstance().player;
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += enemyName + "의 공격! 피해를 " + to_string(max(0, (int)(enemy->GetSTR() - player->GetDEF()))) + " 받았다.";
		screen += "\n";
		screen += "플레이어의 HP가 " + to_string((int)player->GetHP()) + "이(가) 되었다.";
		screen += "\n";
		if (player->IsDead()) {
			screen += "패배했다...\n";
		}
		screen += "SpaceBar: 확인";
	}
	else if (nowState == "GameOverState") {
		screen.clear();
		screen += "================================\n";
		screen += "================================\n";
		screen += "================================\n";
		screen += "================================\n";
		screen += "==========[Game Over]===========\n";
		screen += "================================\n";
		screen += "================================\n";
		screen += "================================\n";
		screen += "================================\n";
	}
	else if (nowState == "TownState") {
		Town* t = GameManager::GetInstance().town;
		screen.clear();
		screen = town;

		screen += "\n";
		screen += "~~~~";
		screen += t->GetName();
		screen += "에 오신 것을 환영합니다~~~~";
		screen += "\n";
		screen += "SpaceBar: 나가기 / (1): 구매 / (2): 판매";
	}
	else if (nowState == "BuyingState") {
		Player* p = GameManager::GetInstance().player;
		screen.clear();
		screen = town;

		screen += "\n";
		screen += "Gold: " + to_string(p->gold) + "G";
		screen += "\n";
		screen += "(1) : 광선검(1000G) / (2) : 최고급 투구(500G) / (3) : 황금 장화(300G)";
		screen += "\n";
		screen += "SpaceBar: 돌아가기";
	}
	else if (nowState == "SellingState") {
		Player* p = GameManager::GetInstance().player;
		screen.clear();
		screen = town;

		screen += "\n";
		screen += "Gold: " + to_string(p->gold) + "G";
		screen += "\n";

		screen += "[판매] ";
		screen += "\n";
		for (int i = 0; i < min(p->inventory.size(), 8); i++) {
			screen += "(" + to_string(i) + ") : " + p->inventory[i]->GetName() + " ";
		}
		if (p->inventory.size() > 8) {
			screen += "(9) : >>다음 아이템>>";
		}
		screen += "\n";
		screen += "SpaceBar: 돌아가기";
	}
	else if (nowState == "WinState") {
		screen.clear();
		screen += "====================================\n";
		screen += "마왕을 처치해서 세상은 평화로워졌다.\n";
		screen += "~~~~~~~~~만세 만세 만만세!!~~~~~~~~~\n";
		screen += "~~~~~~~~~~~~~~~~끝~~~~~~~~~~~~~~~~~~\n";
		screen += "====================================\n";
	}
	else if (nowState == "ThrowingState") {
		Player* player = GameManager::GetInstance().player;
		Character* enemy = GameManager::GetInstance().enemy;
		string enemyName = enemy->GetName();
		DrawMonster(enemyName);

		screen += "\n";
		screen += enemyName + "에게 아이템을 던졌다!";
		screen += "\n";
		screen += enemyName + "의 HP가 " + to_string((int)enemy->GetHP()) + "이(가) 되었다.";
		screen += "\n";
		if (enemy->IsDead()) {
			screen += enemyName + "이(가) 쓰러졌다!\n";
		}
		screen += "SpaceBar: 확인";
	}
	return screen;
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
		}
		break;
	}
	nowPosition = GameManager::GetInstance().player->GetPosition();
	x = nowPosition.x;
	y = nowPosition.y;
	Camera::GetInstance().rig.x = x <= 15 ? 16 : x >= WIDTH - 15 ? WIDTH - 15 : x;
	Camera::GetInstance().rig.y = y <= 15 ? 16 : y >= HEIGHT - 15 ? HEIGHT - 15 : y;
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
	else if (nearObjects.front()->GetType() == MapObject::Type::Equippable || nearObjects.front()->GetType() == MapObject::Type::HPPotion || nearObjects.front()->GetType() == MapObject::Type::MPPotion || nearObjects.front()->GetType() == MapObject::Type::Thrower) {
		MapObject* item = nearObjects.front();
		GameManager::GetInstance().player->GetItem(item);

		int x = item->GetPosition().x;
		int y = item->GetPosition().y;
		item->SetPosition(-1, -1);
		Map::GetInstance().grid[x][y]->occupier = nullptr;
	}
	else if (nearObjects.front()->GetType() == MapObject::Type::Town) {
		Player* player = GameManager::GetInstance().player;
		player->SetHP(player->GetMaxHP());
		player->SetMP(player->GetMaxMP());
		Town* town = (Town*)nearObjects.front();
		GameManager::GetInstance().town = town;
		State* ts = new TownState;
		StateMachine::GetInstance().Transition(*ts);
	}
}

void BattleState::SpaceBarEvent() {
	Player* player = GameManager::GetInstance().player;
	Character* enemy = GameManager::GetInstance().enemy;
	player->Attack(*enemy);

	State* as = new AttackingState;
	StateMachine::GetInstance().Transition(*as);
}

void BattleState::NumberEvent(int i) {
	if (i == 0) {
		Player* player = GameManager::GetInstance().player;
		Consumable* item = nullptr;
		bool isThereThrower = false;
		for (auto iter = player->inventory.begin(); iter != player->inventory.end(); iter++) {
			if ((*iter)->GetName() == "짱돌") {
				item = (Consumable*)*iter;
				GameManager::GetInstance().enemy->Damaged(item->value);
				player->RemoveItem(item);
				delete item;
				isThereThrower = true;
				break;
			}
		}
		if (isThereThrower) {
			State* ts = new ThrowingState;
			StateMachine::GetInstance().Transition(*ts);
		}
	}
	else if (i == 1) {
		Player* player = GameManager::GetInstance().player;
		Consumable* item = nullptr;
		bool isThereThrower = false;
		for (auto iter = player->inventory.begin(); iter != player->inventory.end(); iter++) {
			if ((*iter)->GetName() == "폭탄") {
				item = (Consumable*)*iter;
				GameManager::GetInstance().enemy->Damaged(item->value);
				player->RemoveItem(item);
				delete item;
				isThereThrower = true;
				break;
			}
		}
		if (isThereThrower) {
			State* ts = new ThrowingState;
			StateMachine::GetInstance().Transition(*ts);
		}
	}
}

void AttackingState::OK() {
	Player* player = GameManager::GetInstance().player;
	Character* enemy = GameManager::GetInstance().enemy;
	if (enemy->IsDead()) {
		if (enemy->GetName() == "마왕") {
			State* ws = new WinState;
			StateMachine::GetInstance().Transition(*ws);
		}
		else {
			player->GainEXP((int)enemy->GetMaxHP());
			player->GainGold((int)enemy->gold);
			int x = enemy->GetPosition().x;
			int y = enemy->GetPosition().y;
			Map::GetInstance().grid[x][y]->occupier = nullptr;
			enemy = nullptr;
			delete GameManager::GetInstance().enemy;
			GameManager::GetInstance().enemy = nullptr;

			State* es = new ExploreState;
			StateMachine::GetInstance().Transition(*es);
		}
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
		State* gs = new GameOverState;
		StateMachine::GetInstance().Transition(*gs);
	}
	else {
		State* bs = new BattleState;
		StateMachine::GetInstance().Transition(*bs);
	}
}

void GameOverState::OK() {
	GameManager::GetInstance().IsGameOver = true;
}

void TownState::OK() {
	GameManager::GetInstance().town = nullptr;
	State* es = new ExploreState;
	StateMachine::GetInstance().Transition(*es);
}

void TownState::NumberEvent(int i) {
	if (i == 1) {
		State* bs = new BuyingState;
		StateMachine::GetInstance().Transition(*bs);
	}
	else if (i == 2) {
		State* ss = new SellingState;
		StateMachine::GetInstance().Transition(*ss);
	}
}

void BuyingState::OK() {
	State* ts = new TownState;
	StateMachine::GetInstance().Transition(*ts);
}

void SellingState::OK() {
	State* ts = new TownState;
	StateMachine::GetInstance().Transition(*ts);
}

void BuyingState::NumberEvent(int i) {
	Player* player = GameManager::GetInstance().player;
	if (i == 1) {
		Equippable::ItemOption option1 = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::BaseFlat, 10);
		Equippable::ItemOption option2 = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::TotalMult, 2);
		Equippable* e = new Equippable("광선검", ItemSlot::Type::Weapon, 1000);
		e->AddOption(option1);
		e->AddOption(option2);
		player->Buy(e);
	}
	else if (i == 2) {
		Equippable::ItemOption option1 = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::BaseFlat, 10);
		Equippable::ItemOption option2 = Equippable::ItemOption(Stat::Type::Strength, StatModifier::Type::TotalFlat, 20);
		Equippable* e = new Equippable("최고급 투구", ItemSlot::Type::Head, 500);
		e->AddOption(option1);
		e->AddOption(option2);
		player->Buy(e);
	}
	else if (i == 3) {
		Equippable::ItemOption option = Equippable::ItemOption(Stat::Type::Defense, StatModifier::Type::TotalMult, 2.5);
		Equippable* e = new Equippable("황금 장화", ItemSlot::Type::Leg, 300);
		e->AddOption(option);
		player->Buy(e);
	}
}


void SellingState::NumberEvent(int i) {
	Player* player = GameManager::GetInstance().player;
	if (player->inventory.size() > 8 && i == 9) {
		for (int i = 0; i < 8; i++) {
			MapObject* item = player->inventory.back();
			player->inventory.insert(player->inventory.begin(), item);
			player->inventory.pop_back();
		}
	}
	else if (i > 0 || i < 8) {
		player->Sell(i);
	}
}

void ThrowingState::OK() {
	Player* player = GameManager::GetInstance().player;
	Character* enemy = GameManager::GetInstance().enemy;
	if (GameManager::GetInstance().enemy->IsDead()) {
		if (enemy->GetName() == "마왕") {
			State* ws = new WinState;
			StateMachine::GetInstance().Transition(*ws);
		}
		else {
			player->GainEXP((int)enemy->GetMaxHP());
			player->GainGold((int)enemy->gold);
			int x = enemy->GetPosition().x;
			int y = enemy->GetPosition().y;
			Map::GetInstance().grid[x][y]->occupier = nullptr;
			enemy = nullptr;
			delete GameManager::GetInstance().enemy;
			GameManager::GetInstance().enemy = nullptr;

			State* es = new ExploreState;
			StateMachine::GetInstance().Transition(*es);
		}
	}
	else {
		State* ds = new DefensingState;
		StateMachine::GetInstance().Transition(*ds);
	}
}

int main() {
	GameManager::GetInstance().InitGame();
	GameManager::GetInstance().Play();

	return 0;
}


