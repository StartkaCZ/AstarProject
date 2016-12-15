#pragma once

#include "SDL.h"

// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 100

class FPS_Counter
{
public:
	static FPS_Counter*		Instance();

							FPS_Counter();
							~FPS_Counter();

	void					fpsinit();
	void					fpsthink();

	float					getFramesPerSecond();
	float					getHighestFrame();
	float					getLowestFrame();

private:
	static FPS_Counter*		_instance;

	// An array to store frame times:
	Uint32					_frametimes[FRAME_VALUES];

	// Last calculated SDL_GetTicks
	Uint32					_frametimelast;

	// total frames rendered
	Uint32					_framecount;

	// the value you want
	float					_framespersecond;
	float					_highestFrame;
	float					_lowestFrame;
};

