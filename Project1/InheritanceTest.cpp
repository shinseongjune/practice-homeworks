#include <iostream>

using namespace std;

//============================ Ż?? ???? 1 =============================
namespace Test1 {
	enum Gear {
		P,
		R,
		N,
		D,
		Count
	};

	class Vehicle {
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

	class Bike {
		Gear eGear = P;
		int speed = 0;
		int seat;
	public:
		Bike(int seat) : seat(seat) {
			cout << "Bike Constructed >> " << this << endl;
		}
	
		~Bike() {
			cout << "Bike Destructed >> " << this << endl;
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

	class Motorcycle {
		Gear eGear = P;
		int speed = 0;
		int seat;
		const char* engine;
	public:
		Motorcycle(int seat, const char* engine) : seat(seat), engine(engine) {
			cout << "Motorcycle Constructed >> " << this << endl;
		}

		~Motorcycle() {
			cout << "Motorcycle Destructed >> " << this << endl;
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
		const char* CheckEngine() {
			return engine;
		}
	};

	class Bus {
		Gear eGear = P;
		int speed = 0;
		int seat;
		const char* engine;
	public:
		Bus(int seat, const char* engine) : seat(seat), engine(engine) {
			cout << "Bus Constructed >> " << this << endl;
		}

		~Bus() {
			cout << "Bus Destructed >> " << this << endl;
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
		const char* CheckEngine() {
			return engine;
		}
	};

	class Truck {
		Gear eGear = P;
		int speed = 0;
		int seat;
		const char* engine;
		int maxCapacity;
	public:
		Truck(int seat, const char* engine, int maxCapacity) : seat(seat), engine(engine), maxCapacity(maxCapacity) {
			cout << "Truck Constructed >> " << this << endl;
		}

		~Truck() {
			cout << "Truck Destructed >> " << this << endl;
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
		const char* CheckEngine() {
			return engine;
		}
		int CheckCapacity() {
			return maxCapacity;
		}
	};

	void TestMain() {
		Vehicle vehicle(4);
		Bike bike(2);
		Motorcycle motorcycle(2, "super engine");
		Bus bus(20, "hyper engine");
		Truck truck(2, "awesome engine", 4000);
	}
}

//============================ Ż?? ???? 1 end ==============================

//============================ Ż?? ???? 2 ==================================

namespace Test2 {
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

	class Motorcycle : public Vehicle {
	protected:
		const char* engine;
	public:
		Motorcycle(int seat, const char* engine) : Vehicle(seat) {
			this->engine = engine;
			cout << "Motorcycle Constructed >> " << this << endl;
		}

		~Motorcycle() {
			cout << "Motorcycle Destructed >> " << this << endl;
		}
	};

	class Bus : public Vehicle {
	protected:
		const char* engine;
	public:
		Bus(int seat, const char* engine) : Vehicle(seat) {
			this->engine = engine;
			cout << "Bus Constructed >> " << this << endl;
		}

		~Bus() {
			cout << "Bus Destructed >> " << this << endl;
		}
	};

	class Truck : public Vehicle {
	protected:
		const char* engine;
		int maxCapacity;
	public:
		Truck(int seat, const char* engine, int maxCapacity) : Vehicle(seat) {
			this->engine = engine;
			this->maxCapacity = maxCapacity;
			cout << "Truck Constructed >> " << this << endl;
		}

		~Truck() {
			cout << "Truck Destructed >> " << this << endl;
		}
	};

	void TestMain() {
		Vehicle vehicle(4);
		Bike bike(2);
		Motorcycle motorcycle(2, "super engine");
		Bus bus(20, "hyper engine");
		Truck truck(2, "awesome engine", 4000);
	}
}


//============================ Ż?? ???? 2 end ==============================

void main() {
	cout << "============================== test1 start.========================================" << endl;
	Test1::TestMain();
	cout << "============================== test1 end. test 2 start. ========================================" << endl;
	Test2::TestMain();
	cout << "============================== test2 end. ========================================" << endl;
}