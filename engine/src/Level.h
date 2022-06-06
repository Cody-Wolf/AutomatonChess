#pragma once

class Level
{
private:

public:
	Level() = default;
	virtual void beginLevel() = 0;
	virtual void tick() = 0;
	virtual void endLevel() = 0;
};