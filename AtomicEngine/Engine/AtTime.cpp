#include "AtTime.h"

AtClock Time::RealTime;
AtClock Time::GameTime;

void Time::Start()
{
	GameTime.timeScale = 1.0f;
	GameTime.started = false;

	RealTime.timer.Start();
	RealTime.started = true;
}

AtClock::AtClock()
{
	timeScale = 1.0f;
	paused = false;
}

void AtClock::Start()
{
	paused = false;
	started = true;
	timeScale = 1.0f;
	timer.Start();
}

void AtClock::Stop()
{
	paused = false;
	started = false;
}

void AtClock::Pause()
{
	paused = true;
	timer.Stop();
}

void AtClock::Resume()
{
	paused = false;
	timer.Continue();
}

void AtClock::Step()
{
	dt = (float)dtTimer.Read() / 1000 * timeScale;
	dtTimer.Start();
}

float AtClock::GetTimeSinceStartup()
{
	if (started != false)
		return timer.ReadSec();
	else return 0.0f;
}
