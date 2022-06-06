#pragma once

#include<unordered_map>
#include<functional>
#include<iostream>


#include<GLFW/glfw3.h>

class Level
{
private:

public:
	Level() = default;
	virtual void beginLevel() = 0;
	virtual void tick(float delta) = 0;
	virtual void endLevel() = 0;
};

class DefaultLevel :public Level
{
private:
	float sumTime;
public:
	void beginLevel()
	{
		sumTime = 0;
	}
	void tick(float delta)
	{
		sumTime += delta;
		float r = cos(sumTime * 0.1);
		float g = cos(sumTime * 0.2);
		float b = cos(sumTime * 0.3);
		glClearColor(r, g, b, 1.0f);
	}
	void endLevel()
	{

	}

};



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
Level* nextLevel = new DefaultLevel;
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


class aLevel :public Level
{
private:
	float sumTime;
public:
	void beginLevel()
	{
		sumTime = 0;
	}
	void tick(float delta)
	{
		sumTime += delta;
		float r = cos(sumTime);
		glClearColor(r, 0, 0, 1.0f);
	}
	void endLevel()
	{

	}

};
class bLevel :public Level
{
private:
	float sumTime;
public:
	void beginLevel()
	{
		sumTime = 0;
	}
	void tick(float delta)
	{
		sumTime += delta;
		float g = cos(sumTime);
		glClearColor(0, g, 0, 1.0f);
	}
	void endLevel()
	{

	}

};
class cLevel :public Level
{
private:
	float sumTime;
public:
	void beginLevel()
	{
		sumTime = 0;
	}
	void tick(float delta)
	{
		sumTime += delta;
		float r = cos(sumTime);
		glClearColor(0, 0, r, 1.0f);
	}
	void endLevel()
	{

	}

};