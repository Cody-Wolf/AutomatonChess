#pragma once

#include<unordered_map>
#include<functional>
#include<iostream>

#include<GLFW/glfw3.h>
#include "game.h"

class Level
{
private:

public:
	Level() = default;
	virtual void beginLevel() = 0;
	virtual void tick(GLFWwindow* window,float delta) = 0;
	virtual void endLevel() = 0;
};

class DefaultLevel :public Level
{
private:
	float sumTime;
	WarSystem* sys;
public:
	void beginLevel() {
		sys = new WarSystem(50);
	}
	void tick(GLFWwindow* window,float delta) {
#ifdef _DEBUG
		cout << "===========RoundStart========" << endl;
		cout << "now nums : " << sys->soldiers.size() << endl;
		cout << "state now:\n";
		for (auto x : sys->soldiers)
			cout
			<< "-------------------\n"
			<< "name £º " << x->getName() << endl
			<< "HP : " << x->getHP() << endl
			<< "Level : " << x->getLevel() << endl
			<< "SP : " << x->getSP() << endl
			<< "pos : " << x->getPos().x << ' ' << x->getPos().y << endl;
#endif
		sys->buffManager.loop();
#ifdef _DEBUG
		cout << "==================BUFF end==============" << endl;
#endif
		sys->WarLoop();
		sys->DrawLoop();
	}
	void endLevel() {
		delete sys;
	}
};


Level* currentLevel = nullptr;
Level* nextLevel = nullptr;
template<class T>
void setNextLevel()
{
	nextLevel = new T();
}

bool swapLevel()
{
	if (nextLevel != nullptr)
	{
		if (currentLevel)
		{
			currentLevel->endLevel();
			delete currentLevel;
		}
		currentLevel = nextLevel;
		nextLevel = nullptr;
		currentLevel->beginLevel();
		return true;
	}
	return false;
}

class StartLevel :public Level
{
private:
	std::unordered_map<unsigned int, bool> m_key;
	bool check(GLFWwindow* window, unsigned int key)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS && m_key[key] == 0)
			m_key[key] = 1;
		else
			m_key[key] = 0;
		return m_key[key];
	}
public:
	void beginLevel()
	{

	}
	void tick(GLFWwindow* window, float delta)
	{
		if (check(window, GLFW_KEY_ENTER))
			setNextLevel<DefaultLevel>();
		else if (check(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	void endLevel()
	{

	}
};