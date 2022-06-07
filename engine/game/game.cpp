#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#define SPEED 10.0f
#define FIGHT_RANGE 100
using namespace std;

class Soldier {
	double maxHP, HP, maxSP, SP, exp, level, roundTime;
	int team, cost, drugNum;
	Method* method;
	vector<BUFF*> buffs;
	Soldier* target;
	
public:
	glm::vec2 dir, pos;

	Soldier(int _team) :
		maxHP(100), HP(100), maxSP(100), SP(100),
		method(new Normal), exp(0), level(1),
		drugNum(2), team(_team), roundTime(1), cost(1) {};

	void selectTarget(vector<Soldier*>& soldiers) {
		Soldier* t;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (ans < glm::distance(tmp->pos, pos))
				ans = glm::distance(tmp->pos, pos), t = tmp;
		if (ans < method->range) target = t;
		dir = glm::normalize(t->pos - pos);
	}

	void makeDecision(vector<Soldier*> &soldiers) {
		selectTarget(soldiers);
		if (drugNum && rand() % 100 <= 80 - 100.0 * HP / maxHP){
			takingDurg();
		}
		else if(rand() % 100 <= 50 - 100.0 * HP / maxHP) {
			rest();
		}
		else if (target) {
			attack(target);
		}
		else move();
	}
	void attack(Soldier* target) {}
	void rest() { 
		HP = max(maxHP, HP + maxHP * 0.5); 
		SP = max(maxSP, SP + maxSP * 0.5);
	}
	void takingDurg() {HP = max(maxHP, HP + maxHP * 0.8), drugNum--;}
	void move() { pos = dir * SPEED; }
};

class Wizard :Soldier {

};

class Master : Wizard {

};

class WarSystem {

};

class Method {
public:
	double range, damage;
	virtual void attack(Soldier& target) = 0;
};

class Normal : public Method {
	void attack(Soldier& target) {}
};

class BUFF {

};

signed main(void) {

}