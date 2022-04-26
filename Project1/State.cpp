#include <iostream>

using namespace std;

class Context;

class State {
public:
	virtual void GoNext(Context* c) = 0;
};

class Context {
private:
	State* state;
public:
	void SetState(State* s) {
		state = s;
	}
	void GoNext() {
		state->GoNext(this);
	}
};

class StateOne : public State {
public:
	virtual void GoNext(Context* c);
};

class StateThree : public State {
	virtual void GoNext(Context* c) {
		c->SetState(new StateOne);
		cout << "Go State One" << endl;
		delete this;
	}
};

class StateTwo : public State {
	virtual void GoNext(Context* c) {
		c->SetState(new StateThree);
		cout << "Go State Three" << endl;
		delete this;
	}
};

void StateOne::GoNext(Context* c) {
	c->SetState(new StateTwo);
	cout << "Go State Two" << endl;
	delete this;
}

void main() {
	Context c;
	c.SetState(new StateOne);
	c.GoNext();
	c.GoNext();
	c.GoNext();
}