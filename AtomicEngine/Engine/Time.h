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

	void Step();
	
	float GetTimeSinceStartup();
	float deltaTime(); 

	float timeScale;
	float dt;

	Timer timer;
	Timer dtTimer;
};

class Time {
public:
	static void Init();
	static Clock RealTimeClock;
	static Clock GameTimeClock;
};
