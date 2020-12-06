#include "Timer.h"

struct Clock
{
	Clock();

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
	static Clock RealTime;
	static Clock GameTime;
};
