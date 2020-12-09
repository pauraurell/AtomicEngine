#include "Timer.h"

struct AtClock
{
	AtClock();

	void Start();
	void Stop();

	void Pause();
	void Resume();	
	bool paused;
	bool started;

	float GetTimeSinceStartup();
	void Step();

	float timeScale;
	float dt;

	Timer timer;
	Timer dtTimer;
};

class Time {
public:
	static void Start();
	static AtClock RealTime;
	static AtClock GameTime;
};
