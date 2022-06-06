#pragma once

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

public:
	void beginLevel() override
	{

	}
	void tick(float delta) override
	{

	}
	void endLevel() override
	{

	}
};

Level* nowLevel = new DefaultLevel;
