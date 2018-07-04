#include <windows.h>
#include "mutex.h"

#if !defined(__THREAD)
#define __THREAD

class thread
{
	HANDLE tHandle;
	bool init;

protected:
	volatile bool running;
	volatile bool paused;
	volatile bool iampaused;
	volatile bool stopped;
	volatile bool neverstarted;

public:
	static DWORD WINAPI interrupt(thread *bt)
	{
		while (bt->running == true)
		{
			bt->neverstarted = false;
			if (bt->paused) { bt->iampaused = true; Sleep(100); }
			else
			{
				bt->iampaused = false;
				bt->background(bt);
			}
		}

		bt->stopped = true;

		return (DWORD)0;
	}

	virtual DWORD WINAPI background(thread *bt) = 0;

public:
	thread();
	~thread() { cleanup(); }

	bool start() { init = create(); return init; }
	bool initalised() { return init; }

	void stop();

	void pause(bool s);
	bool isPaused() { return iampaused; }
	bool isRunning() { return running; }

	bool pauseAndWait(bool value, long time, long max);
	bool pauseAndWait(bool value) { return pauseAndWait(value, 500L); }
	bool pauseAndWait(bool value, long time) { return pauseAndWait(value, time, 30L); }

	bool stopAndWait(long time, long max);
	bool stopAndWait(long time) { return stopAndWait(time, 30L); }
	bool stopAndWait() { return stopAndWait(500L); }

	bool WaitLongTimeWithInteruption(long time);

	void destroy();

private:
	bool create();

private:
	void makeNull() { tHandle = NULL; }
	void cleanup();
};

#endif