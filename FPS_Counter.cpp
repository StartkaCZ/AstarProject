#include "FPS_Counter.h"

#include <iostream>
#include <cstring>


FPS_Counter *FPS_Counter::_instance = nullptr;


/**Singleton instance*/
FPS_Counter* FPS_Counter::Instance()
{
	if (_instance == nullptr)
	{//if our instance hasn't been initialized
	 //initialize it
		_instance = new FPS_Counter();
	}
	//return the instance.
	return _instance;
}

FPS_Counter::FPS_Counter()
{
}


FPS_Counter::~FPS_Counter()
{
}

// This function gets called once on startup.
void FPS_Counter::fpsinit()
{
	// Set all frame times to 0ms.
	memset(_frametimes, 0, sizeof(_frametimes));
	_framecount = 0;
	_framespersecond = 0;
	_lowestFrame = 9999;
	_highestFrame = 0;
	_framespersecond = 0;
	_frametimelast = SDL_GetTicks();
}

void FPS_Counter::fpsthink()
{
	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = _framecount % FRAME_VALUES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	_frametimes[frametimesindex] = getticks - _frametimelast;

	// save the last frame time for the next fpsthink
	_frametimelast = getticks;

	// increment the frame count
	_framecount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (_framecount < FRAME_VALUES) 
	{
		count = _framecount;
	}
	else 
	{
		count = FRAME_VALUES;
	}

	// add up all the values and divide to get the average frame time.
	_framespersecond = 0;
	for (i = 0; i < count; i++) 
	{
		_framespersecond += _frametimes[i];
	}

	_framespersecond /= count;

	// now to make it an actual frames per second value...
	_framespersecond = 1000.f / _framespersecond;

	if (_framespersecond > _highestFrame)
	{
		if (_framespersecond != INFINITY)
		{
			_highestFrame = _framespersecond;
		}
	}
	else if (_framespersecond < _lowestFrame)
	{
		_lowestFrame = _framespersecond;
	}
}

float FPS_Counter::getFramesPerSecond()
{
	return _framespersecond;
}
float FPS_Counter::getHighestFrame()
{
	return _highestFrame;
}
float FPS_Counter::getLowestFrame()
{
	return _lowestFrame;
}
