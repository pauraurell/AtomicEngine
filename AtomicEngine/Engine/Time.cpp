#include "Time.h"

Clock Time::RealTimeClock;
Clock Time::GameTimeClock;

void Time::Init()
{
	GameTimeClock.timeScale = 1.0f;
	GameTimeClock.started = false;

	RealTimeClock.timer.Start();
	RealTimeClock.started = true;
}

Clock::Clock()
{
	timeScale = 1.0f;
	paused = false;
}

void Clock::Start()
{
	paused = false;
	started = true;
	timeScale = 1.0f;
	timer.Start();
}

void Clock::Stop()
{
	paused = false;
	started = false;
}

void Clock::Pause()
{
	paused = true;
	timeScale = 0.0f;
	timer.Stop();
}

void Clock::Resume()
{
	paused = false;
	timeScale = 1.0f;
	timer.Resume();
}

void Clock::Step()
{
	dt = (float)dtTimer.Read() / 1000 * timeScale;
	dtTimer.Start();
}

float Clock::GetTimeSinceStartup()
{
	if (started != false)
		return timer.ReadSec();
	else return 0.0f;
}

float Clock::deltaTime()
{
	return dt * timeScale;
}
