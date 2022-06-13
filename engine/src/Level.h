#pragma once

#include<unordered_map>
#include<functional>
#include<iostream>

#include<GLFW/glfw3.h>
#include "game.h"

class Level
{
private:

protected:
	static std::unordered_map<unsigned int, int> m_key;
	static bool check(GLFWwindow* window, unsigned int key)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS && m_key[key] + 200 < clock())
		{
			m_key[key] = clock();
			return true;
		}
		return false;
	}
public:
	Level() = default;
	virtual void beginLevel() = 0;
	virtual void tick(GLFWwindow* window, float delta) = 0;
	virtual void endLevel() = 0;
};
std::unordered_map<unsigned int, int> Level::m_key = std::unordered_map<unsigned int, int>();




//class LevelManger
//{
//private:
//	std::unordered_map<std::string, std::function<Level* ()>> m_Levels;
//public:
//	template<class T>
//	void registerLevel(const std::string& name)
//	{
//		m_Levels[name] = []() {return new T(); };
//	}
//	Level* getLevel(const std::string& name)
//	{
//		auto iter = m_Levels.find(name);
//		if (iter == m_Levels.end())
//		{
//			std::cout << "Level:" << name << "¹Ø¿¨Î´×¢²á" << std::endl;
//			return nullptr;
//		}
//		return iter->second();
//	}
//};

Level* currentLevel = nullptr;
Level* nextLevel = nullptr;
//LevelManger levelManger;

//void setNextLevel(const std::string& level)
//{
//	nextLevel = levelManger.getLevel(level);
//}

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

class StartLevel;
class DefaultLevel :public Level
{
private:
	float sumTime;
	bool isRunning;
	WarSystem* sys;
public:
	void beginLevel() {
		sys = new WarSystem(50), isRunning = 1;
	}
	void tick(GLFWwindow* window, float delta) {
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
		if (isRunning) {
			sys->buffManager.loop();
			sys->WarLoop();
		}
		sys->DrawLoop();
		if (check(window, GLFW_KEY_ESCAPE))
			setNextLevel<StartLevel>();
		if (check(window, GLFW_KEY_SPACE))
			isRunning ^= 1;
		if (sys->isEnd()) setNextLevel<StartLevel>();
	}
	void endLevel() {
		delete sys;
	}
};

class StartLevel :public Level
{
private:
	
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
