#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include"Object.h"

#define SPEED 1.0f
#define FIGHT_RANGE 5
using namespace std;

class BUFF {

};


class Soldier {
protected:
	double maxHP, HP, maxSP, SP, exp, level, roundTime;
	double range, damage;
	int team, cost, drugNum;
	vector<BUFF*> buffs;
	Soldier* target;
	glm::vec2 dir;
	glm::vec2 pos;
	string name;
public:
	Object ob;
	int getTeam()
	{
		return team;
	}
	Soldier(int _team, glm::vec2 _pos) :
		maxHP(100), HP(100), maxSP(100), SP(100),
		exp(0), level(1), pos(_pos), dir(), target(),
		drugNum(2), team(_team), roundTime(1), cost(1),
		damage(20), range(10), name("Soldier") ,ob(makeThreeObject()){}

	virtual void attack(Soldier* target) { 
		cout << "attack" << endl; 
		target->getDamage(damage); 
	}

	void takingDurg() {
		cout << "takingDurg" << endl;
		HP = max(maxHP, HP + maxHP * 0.8), drugNum--;
	}

	void move() {
		cout << "move" << endl; 
		pos += dir * SPEED;
	}

	virtual void rest() {
		cout << "rest" << endl;
		HP = max(maxHP, HP + maxHP * 0.5);
		SP = max(maxSP, SP + maxSP * 0.5);
	}


	virtual void selectTarget(vector<Soldier*>& soldiers) {
		Soldier* t = nullptr;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (tmp->team != team && ans > glm::distance(tmp->pos, pos))
				ans = glm::distance(tmp->getPos(), pos), t = tmp;
		if (ans < range) target = t;
		if (t) dir = glm::normalize(t->getPos() - pos);
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
		cout << "makeDecision" << endl;
		selectTarget(soldiers);
		if (target)
			cout << "target: " << target->pos.x << ' ' << target->pos.y << endl;
		else
			cout << "no target" << endl;
		if (drugNum && rand() % 100 <= 80 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (rand() % 100 <= 60 - 100.0 * HP / maxHP) {
			rest();
		}
		else if (target && SP >= 30) {
			exp += 20, SP -= 30, update();
			attack(target);
			if (!target->isAlive())
				cout << "Kill" << endl;
		}
		else move();
	}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.9;
		range += 1, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}

	void getDamage(double damage) { HP -= damage; }
	bool isAlive() { return HP > 0; }
	glm::vec2 getPos() { return pos; }
	string getName() { return name; }
	double getHP() { return HP; }
};

class Wizard : public Soldier {
	double maxMP, MP;

public:
	Wizard(int _team, glm::vec2 _pos) :
		Soldier(_team, _pos), maxMP(100), MP(100) {
		name = "Wizard"; ob = makeFourObject();
	}

	virtual void attack(Soldier* target) { target->getDamage(damage); }

	virtual void selectTarget(vector<Soldier*>& soldiers) {

	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {

	}

	virtual void update() {

	}

	virtual void rest() {

	}
};


class Master : public Wizard {
	double maxNP, NP;
	Master(int _team, glm::vec2 _pos) :
		Wizard(_team, _pos), maxNP(100), NP(100) {
		name = "Master"; ob = makeFiveObject();
	}

	virtual void attack(Soldier* target) { target->getDamage(damage); }

	virtual void selectTarget(vector<Soldier*>& soldiers) {

	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {

	}

	virtual void update() {

	}

	virtual void rest() {

	}
};

class WarSystem {
public:
	vector<Soldier*> soldiers;
	WarSystem(int _num) {
		for (int i = 0; i < _num; i++)
			soldiers.push_back(new Soldier(rand() % 2, glm::vec2(rand() % 100, rand() % 100)));
	}
	void WarLoop() {for (auto s : soldiers) s->makeDecision(soldiers);}
	void DrawLoop() { 
		for (auto s : soldiers)
		{
			int t = s->getTeam();
			glm::vec3 color;
			if (t == 0)
			{
				color = glm::vec3(1.0f, 0.0f, 0.0f);
			}
			else
			{
				color = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			s->ob.draw(glm::vec3(s->getPos(), 0.0f), glm::vec3(1.0f), color);
		}
	}
};



