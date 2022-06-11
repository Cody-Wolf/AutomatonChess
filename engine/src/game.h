#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_set>
#include "Object.h"

#define SPEED 0.5f
#define FIGHT_RANGE 5
#define BATTLE_RANGE 100
#define TEAM_NUMS 2
#define ROUND_TIME 50
#define REST_CD 500

using namespace std;

static int rangeRand(int l, int r) {
	return rand() % (r - l) + l;
}

class BuffState {
	clock_t endTime;
	bool buffKind;
	string state;
public:
	BuffState(clock_t _endTime, bool _buffKind, string _state) :
		endTime(_endTime), buffKind(_buffKind), state(_state) {}
	string getState() { return state; };
	clock_t getEndTime() { return endTime; }
};

class Soldier {
protected:
	clock_t lastRound, lastRest;
	double maxHP, HP, maxSP, SP, roundTime;
	double range, damage;
	int team, cost, drugNum, level, exp;
	unordered_set<BuffState*> buffs;
	Soldier* target;
	glm::vec2 dir;
	glm::vec2 pos;
	string name;
public:
	Object ob;

	Soldier(int _team, int id, glm::vec2 _pos) :
		maxHP(100), HP(100), maxSP(100), SP(100),
		exp(0), level(1), pos(_pos), dir(), target(),
		drugNum(2), team(_team), roundTime(ROUND_TIME), cost(1), lastRound(clock()), lastRest(clock()),
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

	void updateBuffs() {
		unordered_set<BuffState*> tmp;
		for (auto buff : buffs)
			if (clock() > buff->getEndTime())
				tmp.insert(buff);
			else {
				cout << "BUFF: " << name << ' ' << buff->getState() << endl;
			}

		for (auto buff : tmp)
			cout << "BUFF " << buff->getState() << " ÒÆ³ý" << endl, buffs.erase(buff);
	}

	virtual void rest() {
#ifdef _DEBUG
		cout << "rest" << endl;
#endif
		HP = min(maxHP, HP + maxHP * 0.1);
		SP = min(maxSP, SP + maxSP * 0.2);
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

	virtual void loop(vector<Soldier*>& soldiers) {
		makeDecision(soldiers);
		updateBuffs();
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
			if (glm::distance(target->getPos(), pos) > range)
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
			if (!target->isAlive()) exp += 15;
			update();
		}
		else if (target && glm::distance(target->getPos(), pos) > range && SP >= 1)
			move(), SP -= 1;
		}

	virtual void update() {
		if (exp < 20) return;
		level++, exp -= 20, roundTime *= 0.8;
		range += 1, damage += 20, drugNum++;
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
	int getTeam() { return team; }
	void insertBuff(BuffState* buff) { buffs.insert(buff); }
	void setRoundTime(double x) { roundTime *= x; }
	bool hasBuff() {return buffs.size();}
};

class Buff : public BuffState {
protected:
	Soldier* target;
	bool enable;
public:
	Buff(Soldier* _target, clock_t _endTime, bool _buffKind, string _state) :
		BuffState(_endTime, _buffKind, _state), target(_target), enable(false) {}
	bool isAble() { return enable; }
	virtual void activate() = 0;
	virtual void keep() = 0;
	virtual void lifeEnd() = 0;
};

class BuffManager {
	unordered_set<Buff*> buffs;
public:
	void insert(Buff* buff) { buffs.insert(buff); }
	void loop() {
		unordered_set<Buff*> tmp;
		for (auto buff : buffs) {
			if (!buff->isAble())
				buff->activate();
			if (clock() > buff->getEndTime())
				buff->lifeEnd(), tmp.insert(buff);
			else
				buff->keep();
		}
		for (auto buff : tmp) buffs.erase(buff);
	}
};

class LifeLine : public Buff {
public:
	LifeLine(Soldier* _target, clock_t _endTime) :
		Buff(_target, _endTime, 1, "»ØÑª") {}

	void activate() {}
	void keep() { target->getDamage(-10); }
	void lifeEnd() {}
};

class SpeedUp : public Buff {
public:
	SpeedUp(Soldier* _target, clock_t _endTime) :
		Buff(_target, _endTime, 1, "¼ÓËÙ") {}
	void activate() { target->setRoundTime(0.05); }
	void keep() {}
	void lifeEnd() { target->setRoundTime(20); }
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
		if (glm::distance(pos, target->getPos()) < 1) {
			for (auto tmp : soldiers)
				if (tmp->getTeam() && tmp->getTeam() != frd && glm::distance(tmp->getPos(), pos) < range)
					tmp->getDamage(damage);
			HP = -1;
		}
		else move();
	}

};

class Wizard : public Soldier {
protected:
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
		else if (target && SP >= 5)
			move(), SP -= 5;
	}

	virtual void rest() {
#ifdef _DEBUG
		cout << "rest" << endl;
#endif
		HP = min(maxHP, HP + maxHP * 0.1);
		SP = min(maxSP, SP + maxSP * 0.1);
		MP = min(maxMP, MP + maxMP * 0.3);
}


	virtual void update() {
		if (exp < 40) return;
		level++, exp -= 40, roundTime *= 0.9;
		range += 0.2, damage += 20, drugNum++;
		HP *= 1.2, maxHP *= 1.2, maxSP *= 1.2;
	}
};

class Master : public Soldier {
protected:
	double maxNP, NP;
	BuffManager* buffmanager;
public:
	Master(int _team, int id, glm::vec2 _pos, BuffManager* _buffmanager) :
		Soldier(_team, id, _pos), maxNP(100), NP(100), buffmanager(_buffmanager) {
		name = "Master"; ob = makeFiveObject(); range = 100;
	}

	void selectTarget(vector<Soldier*>& soldiers) {
		dir = glm::vec2(0, 0);
		Soldier* t = nullptr;
		double ans = -1;
		for (auto tmp : soldiers)
			if (tmp->getTeam() == team && ans < glm::distance(tmp->getPos(), pos))
				ans = glm::distance(tmp->getPos(), pos), t = tmp;
		target = t;
	}

	void attack(Soldier* target) {
		Buff* buff = new SpeedUp(target, clock() + 1000);
		target->insertBuff(buff);
		buffmanager->insert(buff);
	}

};

class WarSystem {
public:
	vector<Soldier*> soldiers;
	BuffManager buffManager;
	WarSystem(int _num) {
		int WizardNums = 0.05 * _num;
		for (int i = 0; i < WizardNums; i++)
			soldiers.push_back(new Wizard(1, i, glm::vec2(rangeRand(0, BATTLE_RANGE / 4), rangeRand(10, BATTLE_RANGE))));
		for (int i = 0; i < WizardNums; i++)
			soldiers.push_back(new Wizard(2, i, glm::vec2(rangeRand(BATTLE_RANGE / 4 * 3, BATTLE_RANGE), rangeRand(10, BATTLE_RANGE))));

		for (int i = 0; i < _num - WizardNums; i++)
			soldiers.push_back(new Soldier(1, i, glm::vec2(rangeRand(0, BATTLE_RANGE / 4), rangeRand(10, BATTLE_RANGE))));
		for (int i = 0; i < _num - WizardNums; i++)
			soldiers.push_back(new Soldier(2, i, glm::vec2(rangeRand(BATTLE_RANGE / 4 * 3, BATTLE_RANGE), rangeRand(10, BATTLE_RANGE))));

		soldiers.push_back(new Master(1, 1, glm::vec2(rangeRand(0, BATTLE_RANGE / 4), rangeRand(10, BATTLE_RANGE)), &buffManager));
		soldiers.push_back(new Master(2, 1, glm::vec2(rangeRand(BATTLE_RANGE / 4 * 3, BATTLE_RANGE), rangeRand(10, BATTLE_RANGE)), &buffManager));
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
				soldiers[i]->loop(soldiers), tmp.push_back(soldiers[i]);
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
			}
			else if (t == 1)
			{
				color = glm::vec3(1.0f, 0.0f, 0.0f);
				if(s->hasBuff())
					color = glm::vec3(255.0f / 255, 219.0f / 255, 0.0f / 255);
				s->ob.draw(glm::vec3(s->getPos(), 0.0f), glm::vec3(1.0f), color * s->getRatioHP());
			}
			else
			{
				color = glm::vec3(0.0f, 1.0f, 0.0f);
				if (s->hasBuff())
					color = glm::vec3(85.0f / 255, 170.0f / 255, 255.0f / 255);
				s->ob.draw(glm::vec3(s->getPos(), 0.0f), glm::vec3(1.0f), color * s->getRatioHP());
			}
			float dx = s->getDir().x;
			float dy = s->getDir().y;
			float angle = atan2(dy, -dx);
			s->ob.draw(glm::vec3(s->getPos(), 0.0f), angle, color * s->getRatioHP());
		}
	}
};

