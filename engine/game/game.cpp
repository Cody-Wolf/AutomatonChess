#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#define SPEED 10.0f
#define FIGHT_RANGE 100
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

public:
	glm::vec2 pos;

	Soldier(int _team, glm::vec2 _pos) :
		maxHP(100), HP(100), maxSP(100), SP(100),
		exp(0), level(1), pos(_pos), dir(), target(),
		drugNum(2), team(_team), roundTime(1), cost(1),
		damage(20), range(10) {}

	virtual void attack(Soldier* target) { target->getDamage(damage); }

	virtual void selectTarget(vector<Soldier*>& soldiers) {
		Soldier* t = nullptr;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (tmp->team != team && ans < glm::distance(tmp->pos, pos))
				ans = glm::distance(tmp->pos, pos), t = tmp;
		if (ans < range) target = t;
		if (t) dir = glm::normalize(t->pos - pos);
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
		selectTarget(soldiers);
		if (drugNum && rand() % 100 <= 80 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (rand() % 100 <= 60 - 100.0 * HP / maxHP) {
			rest();
		}
		else if (target && SP >= 30) {
			exp += 20, SP -= 30, update();
			attack(target);
		}
		else move();
	}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.9;
		range += 1, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}

	virtual void rest() {
		HP = max(maxHP, HP + maxHP * 0.5);
		SP = max(maxSP, SP + maxSP * 0.5);
	}

	void takingDurg() { HP = max(maxHP, HP + maxHP * 0.8), drugNum--; }

	void move() { pos = dir * SPEED; }

	void getDamage(double damage) { HP -= damage; }

	bool isAlive() { return HP > 0; }
};

class Wizard : public Soldier {
	double maxMP, MP;

public:
	Wizard(int _team, glm::vec2 _pos) :
		Soldier(_team, _pos), maxMP(100), MP(100) {}

	virtual void attack(Soldier* target) { target->getDamage(damage); }

	virtual void selectTarget(vector<Soldier*>& soldiers) {
		Soldier* t = nullptr;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (ans < glm::distance(tmp->pos, pos))
				ans = glm::distance(tmp->pos, pos), t = tmp;
		if (ans < range) target = t;
		if (t) dir = glm::normalize(t->pos - pos);
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
		selectTarget(soldiers);
		if (drugNum && rand() % 100 <= 80 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (rand() % 100 <= 60 - 100.0 * HP / maxHP) {
			rest();
		}
		else if (target && SP >= 30) {
			exp += 20, SP -= 30, update();
			attack(target);
		}
		else move();
	}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.9;
		range += 1, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}

	virtual void rest() {
		HP = max(maxHP, HP + maxHP * 0.5);
		SP = max(maxSP, SP + maxSP * 0.5);
	}
};


class Master : public Wizard {
	double maxNP, NP;
	Master(int _team, glm::vec2 _pos) :
		Wizard(_team, _pos), maxNP(100), NP(100) {}

	virtual void attack(Soldier* target) { target->getDamage(damage); }

	virtual void selectTarget(vector<Soldier*>& soldiers) {
		Soldier* t = nullptr;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (ans < glm::distance(tmp->pos, pos))
				ans = glm::distance(tmp->pos, pos), t = tmp;
		if (ans < range) target = t;
		if (t) dir = glm::normalize(t->pos - pos);
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
		selectTarget(soldiers);
		if (drugNum && rand() % 100 <= 80 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (rand() % 100 <= 60 - 100.0 * HP / maxHP) {
			rest();
		}
		else if (target && SP >= 30) {
			exp += 20, SP -= 30, update();
			attack(target);
		}
		else move();
	}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.9;
		range += 1, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}

	virtual void rest() {
		HP = max(maxHP, HP + maxHP * 0.5);
		SP = max(maxSP, SP + maxSP * 0.5);
	}
};

class WarSystem {

};
