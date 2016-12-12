#ifndef GAME_H
#define GAME_H

#include "Debug.h"
#include "Player.h"
#include "Tile.h"
#include "NPC.h"
#include "Level.h"
#include "Camera.h"
#include "Grid.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>

class Game
{
private:
	struct Data
	{
		Data(Grid* g, queue<NPC*>& j)
			: grid(g)
			, jobs(j)
		{

		}

		Grid*				grid;
		queue<NPC*>&		jobs;
	};

	struct Logger
	{
		Logger(map<string, int>* l)
			: threadJobDoneCounter(l)
		{

		}

		Data* data;

		map<string, int>*		threadJobDoneCounter;
		string					threadName;
	};

public:
							Game();
							~Game();

	bool					Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	bool					SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);

	void					Render();
	void					Update();
	void					HandleEvents();
	bool					IsRunning();
	void					CleanUp();

	static int				Worker(void*);

private:
	bool					_running;

	SDL_Window*				_window;
	SDL_Renderer*			_renderer;

	Grid*					_Grid;
	Level*					_level;
	Camera*					_camera;
	Player*					_player;

	vector<NPC*>			_npcs;
	queue<NPC*>				_jobs;

	map<string, int>*		_threadJobDoneLog;

	unsigned int			_lastTime;//time of last update;
	int						_currentLevel;

	int						_arrive[8] = { 0 };
	int						_continue[8] = { 0 };

	static SDL_semaphore*	_semaphore;
};



#endif

