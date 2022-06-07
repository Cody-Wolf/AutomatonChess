#include <iostream>
#include <vector>
using namespace std;

class Soldier {
	double maxHP, HP, maxSP, SP, exp, level, roundTime;
	int team, cost, drugNum;
	Method* method;
	vector<BUFF*> buffs;
public:
	Soldier(int _team) :
		maxHP(100), HP(100), maxSP(100), SP(100),
		method(new Normal), exp(0), level(1),
		drugNum(2), team(_team), roundTime(1), cost(1) {};
	void makeDecision() {}
	void attack() {}
	void rest() {}
	void takingDurg() {}
	void move() {}
};

class Wizard :Soldier {

};

class Master : Wizard {

};

class WarSystem {

};

class Method {
	virtual void attack(Soldier& target) = 0;
};

class Normal : public Method {
	double range, damage;
	void attack(Soldier& target) {}
};

class BUFF {

};

signed main(void) {

}