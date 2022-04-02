#include <iostream>
#include <list>

using namespace std;

class Point {
public:
	int x;
	int y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(int x, int y) : x(x), y(y) {

	}
};

class Shape {
public:
	Shape() {
		cout << "Shape Constructed >> " << this << endl;
	}
	virtual ~Shape() = 0 {
		cout << "Shape Destructed >> " << this << endl;
	}
	void Draw() {
		cout << "Shape Draw!" << endl;
	}
};

class Circle : public Shape {
	Point center;
	int radius;
public:
	Circle(int x, int y, int radius) {
		center = Point(x, y);
		this->radius = radius;
		cout << "Circle Constructed >> " << this << endl;
	}
	~Circle() {
		cout << "Circle Destructed >> " << this << endl;
	}

	void Draw() {
		cout << "Circle Draw!! center :" << center.x << "," << center.y << " radius :" << radius << endl;
	}
};

class Rectangle : public Shape {
	Point bottomLeft;
	Point topRight;
public:
	Rectangle(int a, int b, int c, int d) {
		bottomLeft = Point(a, b);
		topRight = Point(c, d);
		cout << "Rectangle Constructed >> " << this << endl;
	}
	~Rectangle() {
		cout << "Rectangle Destructed >> " << this << endl;
	}

	void Draw() {
		cout << "Rectangle Draw!! bottomLeft :" << bottomLeft.x << "," << bottomLeft.y << " topRight :" << topRight.x << "," << topRight.y << endl;
	}
};

class Triangle : public Shape {
	Point a;
	Point b;
	Point c;
public:
	Triangle(int a, int b, int c, int d, int e, int f) {
		this->a = Point(a, b);
		this->b = Point(c, d);
		this->c = Point(e, f);
		cout << "Triangle Constructed >> " << this << endl;
	}
	~Triangle() {
		cout << "Triangle Destructed >> " << this << endl;
	}

	void Draw() {
		cout << "Triangle Draw!! A :" << a.x << "," << a.y << " B :" << b.x << "," << b.y << " C :" << c.x << "," << c.y << endl;
	}
};

void TestMain() {
	list<Shape*> lt;
	Circle* c = new Circle(0, 0, 3);
	Rectangle* r = new Rectangle(1, 1, 5, 5);
	Triangle* t = new Triangle(3, 3, 5, 5, 3, 9);
	lt.push_back(c);
	lt.push_back(r);
	lt.push_back(t);

	auto iter = lt.begin();
	for (; iter != lt.end(); iter++) {
		(*iter)->Draw();
	}

	for (iter = lt.begin(); iter != lt.end(); iter++) {
		delete* iter;
	}
}

void main() {
	TestMain();
}