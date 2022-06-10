#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include"Object.h"

#define SPEED 0.5f
#define FIGHT_RANGE 6
#define BATTLE_RANGE 100
#define TEAM_NUMS 2
#define ROUND_TIME 50
#define REST_CD 1000

using namespace std;

static int rangeRand(int l, int r) {
	return rand() % (r - l) + l;
}

class BUFF {

};


class Soldier {
protected:
	clock_t lastRound, lastRest;
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
		drugNum(2), team(_team), roundTime(ROUND_TIME), cost(1), lastRound(clock()),lastRest(clock()),
		damage(20), range(FIGHT_RANGE), name("Soldier" + to_string(id) + " " + to_string(team)), ob(makeThreeObject()) {}

	virtual void attack(Soldier* target) {
#ifdef _DEBUG
		cout << "attack" << endl;
#endif
		target->getDamage(damage);
	}

	void takingDurg() {
#ifdef _DEBUG
		cout << "takingDurg" << endl;
#endif
		HP = min(maxHP, HP + maxHP * 0.8), drugNum--;
	}

	void move() {
#ifdef _DEBUG
		cout << "move" << endl;
#endif
		pos += dir * SPEED;
		pos.x = min(1.0f * BATTLE_RANGE, pos.x);
		pos.x = max(0.0f, pos.x);
		pos.y = min(1.0f * BATTLE_RANGE, pos.y);
		pos.y = max(0.0f, pos.y);
	}

	virtual void rest() {
#ifdef _DEBUG
		cout << "rest" << endl;
#endif
		HP = min(maxHP, HP + maxHP * 0.01);
		SP = min(maxSP, SP + maxSP * 0.1);
	}


	virtual void selectTarget(vector<Soldier*>& soldiers) {
		dir = glm::vec2(0, 0);
		Soldier* t = nullptr;
		double ans = 1e16;
		for (auto tmp : soldiers)
			if (tmp->getTeam() != team && tmp->getTeam() && ans > glm::distance(tmp->getPos(), pos))
				ans = glm::distance(tmp->getPos(), pos), t = tmp;
		target = t;
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
#ifdef _DEBUG
		cout << "makeDecision" << endl;
		cout << "now time : " << clock() << endl;
#endif
		if (clock() - lastRound >= roundTime)
			lastRound += roundTime;
		else
			return;

		if (clock() - lastRest > REST_CD) {
			lastRest = clock();
			rest();
		}

		if (!target || !target->isAlive()) selectTarget(soldiers);
		if (target) {
			if (glm::distance(target->getPos(), pos) > range * 0.5)
				dir = glm::normalize(target->getPos() - pos);
			else
				dir = glm::normalize(pos - target->getPos());
		}
#ifdef _DEBUG
		if (target) cout << "dis : " << glm::distance(target->getPos(), pos) << endl;
		if (target)
			cout << "target: " << target->pos.x << ' ' << target->pos.y << endl;
		else
			cout << "no target" << endl;
#endif
		if (drugNum > 0 && rand() % 100 <= 50 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (target && SP >= 30 && glm::distance(target->getPos(), pos) <= range) {
			SP -= 30, exp += 1, attack(target);
			if (!target->isAlive()) exp += 15, drugNum++;
			update();
		}
		else if (target && SP >= 1)
			move(), SP -= 1;
	}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.75;
		range += 0.2, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}

	void getDamage(double damage) { HP -= damage; }
	bool isAlive() { return HP > 0; }
	glm::vec2 getPos() { return pos; }
	glm::vec2 getDir() { return dir; }
	string getName() { return name; }
	double getHP() { return HP; }
	int getLevel() { return level; }
	int getSP() { return SP; }
	float getRatioHP() { return HP / maxHP; }
};

class Magic : public Soldier {
public:
	int frd;
	Magic(double _damage, int team, glm::vec2 _pos, Soldier* _target) : Soldier(0, 0, _pos), frd(team) {
		target = _target, range = 10, name = "Magic";
		roundTime = 1, damage = _damage, ob = makeCubeObject();
	}
	void makeDecision(vector<Soldier*>& soldiers) {
		dir = glm::normalize(target->getPos() - pos);
		if (glm::distance(pos, target->getPos()) < 2) {
			for (auto tmp : soldiers)
				if (tmp->getTeam() && tmp->getTeam() != frd && glm::distance(tmp->getPos(), pos) < range)
					tmp->getDamage(damage);
			HP = -1;
		}
		else move();
	}

};

class Wizard : public Soldier {
	double maxMP, MP;

public:
	Wizard(int _team, int id, glm::vec2 _pos) :
		Soldier(_team, id, _pos), maxMP(100), MP(100) {
		name = "Wizard"; ob = makeFourObject(), range = FIGHT_RANGE * 8;
		damage = 80;
	}

	virtual void makeDecision(vector<Soldier*>& soldiers) {
		if (clock() - lastRound >= roundTime)
			lastRound += roundTime;
		else
			return;

		if (clock() - lastRest > REST_CD) {
			lastRest = clock();
			rest();
		}

		if (!target || !target->isAlive()) selectTarget(soldiers);
		if (target) {
			if (glm::distance(target->getPos(), pos) > range)
				dir = glm::normalize(target->getPos() - pos);
			else
				dir = glm::normalize(pos - target->getPos());
		}
		if (drugNum > 0 && rand() % 100 <= 50 - 100.0 * HP / maxHP) {
			takingDurg();
		}
		else if (target && MP >= 50 && glm::distance(target->getPos(), pos) <= range) {
			MP -= 50, exp += 5;
			soldiers.push_back(new Magic(damage, team, pos + glm::vec2(1, 1), target));
			update();
		}
		else if (target && SP >= 50)
			move(), SP -= 50;
	}

	virtual void rest() {
#ifdef _DEBUG
		cout << "rest" << endl;
#endif
		HP = min(maxHP, HP + maxHP * 0.05);
		SP = min(maxSP, SP + maxSP * 0.1);
		MP = min(maxMP, MP + maxMP * 0.03);
	}


	virtual void update() {
		if (exp < 50) return;
		level++, exp -= 50, roundTime *= 0.8;
		range += 0.2, damage += 20;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
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



	virtual void rest() {

	}
};

class WarSystem {
public:
	vector<Soldier*> soldiers;
	WarSystem(int _num) {
		int WizardNums = 0.05 * _num;
		for (int i = 0; i < WizardNums; i++)
			soldiers.push_back(new Wizard(1, i, glm::vec2(rangeRand(0, BATTLE_RANGE / 2), rangeRand(10, BATTLE_RANGE))));
		for (int i = 0; i < WizardNums; i++)
			soldiers.push_back(new Wizard(2, i, glm::vec2(rangeRand(BATTLE_RANGE / 2, BATTLE_RANGE), rangeRand(10, BATTLE_RANGE))));

		for (int i = 0; i < _num - WizardNums; i++)
			soldiers.push_back(new Soldier(1, i, glm::vec2(rangeRand(0, BATTLE_RANGE / 2), rangeRand(10, BATTLE_RANGE)))); 
		for (int i = 0; i < _num - WizardNums; i++)
			soldiers.push_back(new Soldier(2, i, glm::vec2(rangeRand(BATTLE_RANGE / 2, BATTLE_RANGE), rangeRand(10, BATTLE_RANGE))));

		/*soldiers.push_back(new Wizard(1, 1, glm::vec2(rand() % BATTLE_RANGE, rand() % BATTLE_RANGE)));
		soldiers.push_back(new Wizard(2, 2, glm::vec2(rand() % BATTLE_RANGE, rand() % BATTLE_RANGE)));
		for (int i = 0; i < 6; i++)
			soldiers.push_back(new Wizard(rand() % TEAM_NUMS + 1, i, glm::vec2(rand() % (BATTLE_RANGE / 2, rand() % BATTLE_RANGE)));
		for (int i = 0; i < _num - 6; i++)
			soldiers.push_back(new Soldier(rand() % TEAM_NUMS + 1, i, glm::vec2(rand() % BATTLE_RANGE, rand() % BATTLE_RANGE)));*/
	}
	void WarLoop() {
		vector<Soldier*> tmp;
		for (int i = 0; i < soldiers.size(); i++) {
			if (soldiers[i]->isAlive())
				soldiers[i]->makeDecision(soldiers), tmp.push_back(soldiers[i]);
		}
		soldiers = tmp;
	}
	void DrawLoop() {
		for (auto s : soldiers)
		{
			int t = s->getTeam();
			glm::vec3 color;
			if (t == 0) {
				color = glm::vec3(245.0f / 255, 150.0f / 255, 170.0f / 255); //245 150 170
				s->ob.draw(glm::vec3(s->getPos(), 1.5f), glm::vec3(1.0f), color * s->getRatioHP());
			}
			else if (t == 1)
			{
				color = glm::vec3(1.0f, 0.0f, 0.0f);
				s->ob.draw(glm::vec3(s->getPos(), 0.0f), glm::vec3(1.0f), color * s->getRatioHP());
			}
			else
			{
				color = glm::vec3(0.0f, 1.0f, 0.0f);
				s->ob.draw(glm::vec3(s->getPos(), 0.0f), glm::vec3(1.0f), color * s->getRatioHP());
			}

		}
	}
};



