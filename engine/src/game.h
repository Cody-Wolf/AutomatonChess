#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include"Object.h"

#define SPEED 1.0f
#define FIGHT_RANGE 10
#define BATTLE_RANGE 100
#define TEAM_NUMS 2

using namespace std;

class BUFF {

};


class Soldier {
protected:
	double maxHP, HP, maxSP, SP, roundTime;
	double range, damage;
	int team, cost, drugNum, level, exp;
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
	Soldier(int _team, int id, glm::vec2 _pos) :
		maxHP(100), HP(100), maxSP(100), SP(100),
		exp(0), level(1), pos(_pos), dir(), target(),
		drugNum(2), team(_team), roundTime(1), cost(1),
		damage(20), range(1), name("Soldier" + to_string(id) + " " + to_string(team)), ob(makeThreeObject()) {}

	virtual void attack(Soldier* target) {
		cout << "attack" << endl;
		cout << "attack before : " << target->getHP() << endl;
		target->getDamage(damage);
		cout << "attack after : " << target->getHP() << endl;
	}

	void takingDurg() {
		cout << "takingDurg" << endl;
		HP = min(maxHP, HP + maxHP * 0.8), drugNum--;
	}

	void move() {
		cout << "move" << endl;
		pos += dir * SPEED;
		pos.x = min(1.0f * BATTLE_RANGE, pos.x);
		pos.x = max(0.0f, pos.x);
		pos.y = min(1.0f * BATTLE_RANGE, pos.y);
		pos.y = max(0.0f, pos.y);
	}

	virtual void rest() {
		cout << "rest" << endl;
		HP = min(maxHP, HP + maxHP * 0.05);
		SP = min(maxSP, SP + maxSP * 0.2);
	}


	virtual void selectTarget(vector<Soldier*>& soldiers) {
		dir = glm::vec2(0, 0);
		Soldier* t = nullptr;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (tmp->getTeam() != team && ans > glm::distance(tmp->getPos(), pos))
				ans = glm::distance(tmp->getPos(), pos), t = tmp;
		target = t;
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
		cout << "makeDecision" << endl;
		if(!target || !target->isAlive()) selectTarget(soldiers);
		if (target) dir = glm::normalize(target->getPos() - pos);
		/*if (target) cout << "dis : " << glm::distance(target->getPos(), pos) << endl;
		if (target)
			cout << "target: " << target->pos.x << ' ' << target->pos.y << endl;
		else
			cout << "no target" << endl;*/
		if (drugNum > 0 && rand() % 100 <= 50 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (rand() % 100 <= 30 - 100.0 * HP / maxHP || rand() % 100 <= 80 - 100.0 * SP / maxSP) {
			rest();
		}
		else if (target && SP >= 30 && glm::distance(target->getPos(), pos) <= range) {
			SP -= 30, exp += 2, attack(target);
			if (!target->isAlive())
				cout << "Kill" << endl, exp += 20;
			update();
		}
		else if (target && SP >= 1)
			move(), SP -= 1;
		else rest();
	}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.9;
		range += 0.5, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}

	void getDamage(double damage) { HP -= damage; }
	bool isAlive() { return HP > 0; }
	glm::vec2 getPos() { return pos; }
	string getName() { return name; }
	double getHP() { return HP; }
	int getLevel() { return level; }
	int getSP() { return SP; }
};

class Wizard : public Soldier {
	double maxMP, MP;

public:
	Wizard(int _team, int id, glm::vec2 _pos) :
		Soldier(_team, id,  _pos), maxMP(100), MP(100) {
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
	Master(int _team, int id, glm::vec2 _pos) :
		Wizard(_team, id, _pos), maxNP(100), NP(100) {
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
			soldiers.push_back(new Soldier(rand() % TEAM_NUMS, i, glm::vec2(rand() % BATTLE_RANGE, rand() % BATTLE_RANGE)));
	}
	void WarLoop() {
		for (auto it = soldiers.begin(); it != soldiers.end();) {
			if ((*it)->isAlive())
				(*it)->makeDecision(soldiers), it++;
			else
				it = soldiers.erase(it);
		}
	}
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



