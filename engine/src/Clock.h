#pragma once

#include<xtimec.h>

namespace cng
{

class Clock
{
private:
	long long m_start;
	long long m_last;
	float m_Speed;
	bool is_Pause;
	static const long long t_hz;
	inline static float tickToSecond(long long Ticks)
	{
		return Ticks * 1.0f / t_hz;
	}
public:
	Clock(float Speed = 1.0f, bool Pause = false) :m_Speed(Speed), is_Pause(Pause)
	{
		reStart();
	}
	inline float getDeltaSeconds()
	{
		long long now = _Query_perf_counter();
		float res = tickToSecond((now - m_last) * (!is_Pause)) * m_Speed;
		m_last = now;
		return res;
	}
	inline float getCountSeconds()//wrong
	{
		long long now = _Query_perf_counter();
		return tickToSecond((now - m_last) * (!is_Pause) + (m_last - m_start)) * m_Speed;
	}
	inline void reStart()
	{
		m_start = m_last = _Query_perf_counter();
	}
	inline bool isPause()
	{
		return is_Pause;
	}
	inline void setPause(bool Pause = true)
	{
		is_Pause = Pause;
	}
	inline void setContinue()
	{
		is_Pause = false;
	}
	inline void setSpeed(float Speed)
	{
		m_Speed = Speed;
	}
};

const long long Clock::t_hz = _Query_perf_frequency();

}