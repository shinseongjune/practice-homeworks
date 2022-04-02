#include <iostream>

using namespace std;

class Vehicle {
protected:
	enum Gear {
		P,
		R,
		N,
		D,
		Count
	};
	Gear eGear = P;
	int speed = 0;
	int seat;
public:
	Vehicle(int seat) : seat(seat)
	{
		cout << "Vehicle Constructed >> " << this << endl;
	}

	~Vehicle() {
		cout << "Vehicle Destructed >> " << this << endl;
	}

	void Accel() {
		speed++;
	}
	void Break() {
		speed--;
		if (speed < 0) speed = 0;
	}
	void ChangeGear(int gear) {
		if (gear < 0 || gear >= (int)Gear::Count) return;
		eGear = (Gear)gear;
	}
};

class Bike : public Vehicle {
public:
	Bike(int seat) : Vehicle(seat) {
		cout << "Bike Constructed >> " << this << endl;
	}

	~Bike() {
		cout << "Bike Destructed >> " << this << endl;
	}
};

class Enginer : public Vehicle {
protected:
	const char* engine;
public:
	Enginer(int seat, const char* engine) : Vehicle(seat) {
		this->engine = engine;
		cout << "Enginer Constructed >> " << this << endl;
	}

	~Enginer() {
		cout << "Enginer Destructed >> " << this << endl;
	}
};

class Bus : public Enginer {
public:
	Bus(int seat, const char* engine) : Enginer(seat, engine) {
		cout << "Bus Constructed >> " << this << endl;
	}

	~Bus() {
		cout << "Bus Destructed >> " << this << endl;
	}
};

class Truck : public Enginer {
protected:
	int maxCapacity;
public:
	Truck(int seat, const char* engine, int maxCapacity) : Enginer(seat, engine) {
		this->maxCapacity = maxCapacity;
		cout << "Truck Constructed >> " << this << endl;
	}

	~Truck() {
		cout << "Truck Destructed >> " << this << endl;
	}
};

class Motorcycle : public Bike, Enginer {
public:
	Motorcycle(int seat, const char* engine) : Bike(seat), Enginer(seat, engine) {
		cout << "Motorcycle Constructed >> " << this << endl;
	}

	~Motorcycle() {
		cout << "Motorcycle destructed >> " << this << endl;
	}

	/*int GetSeat() {
		return seat;
	}*/
};

void TestMain() {
	Vehicle vehicle(4);
	Bike bike(2);
	Enginer enginer(2, "super engine");
	Bus bus(20, "hyper engine");
	Truck truck(2, "awesome engine", 4000);

	Motorcycle motorcycle(1, "ultra engine");
}

void main() {
	TestMain();
}