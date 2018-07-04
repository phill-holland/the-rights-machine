#include "thread.h"

thread::thread()
{
	init = false;

	makeNull();

	running = false; stopped = false;
	paused = false; iampaused = false;

	neverstarted = true;
}

void thread::stop()
{
	if (running) running = false;
	else if (neverstarted) stopped = true;
}

void thread::pause(bool s)
{
	paused = s;
}

bool thread::pauseAndWait(bool value, long time, long max)
{
	pause(value);

	if ((value) && (running))
	{
		long error = 0L;
		while ((!isPaused()) && (error<max))
		{
			Sleep(time);
			++error;
		}

		if (error >= max) return false;

	}

	return true;
}

bool thread::stopAndWait(long time, long max)
{
	stop();

	long error = 0L;
	while ((!stopped) && (error < max))
	{
		Sleep(time);
		++error;
	}

	if (error >= max) return false;

	return true;
}

bool thread::WaitLongTimeWithInteruption(long time)
{
	const int wait = 200;
	int loops = time / wait;
	int counter = 0;

	while (!isPaused() && isRunning() && (counter < loops))
	{
		Sleep(wait);
		++counter;
	}

	return counter == loops;
}

void thread::destroy()
{
	running = false;
	if (tHandle != NULL) TerminateThread(tHandle, 0);
	tHandle = NULL;
}

bool thread::create()
{
	running = true; stopped = false;
	paused = false; iampaused = false;

	tHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)interrupt, this, 0, NULL);
	if (tHandle == NULL) return false;

	return true;
}

void thread::cleanup()
{
	if (stopAndWait())
	{
		if (tHandle != NULL) CloseHandle(tHandle);
	}
	else destroy();
}