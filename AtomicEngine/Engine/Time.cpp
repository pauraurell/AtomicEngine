#include "Time.h"

Clock Time::RealTime;
Clock Time::GameTime;

void Time::Start()
{
	GameTime.timeScale = 1.0f;
	GameTime.started = false;

	RealTime.timer.Start();
	RealTime.started = true;
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
	timer.Stop();
}

void Clock::Resume()
{
	paused = false;
	timer.Continue();
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
