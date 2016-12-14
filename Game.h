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
	struct Job
	{
		NPC* _npc;

		int playerX;
		int playerY;
	};

	struct Data
	{
		Data()
			: grid(nullptr)
			, jobs(queue<Job*>())
		{

		}

		Grid*			grid;
		queue<Job*>		jobs;

		int tileSize;
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
	
	void					Render();
	void					Update();
	void					HandleEvents();
	bool					IsRunning();
	void					CleanUp();

	static int				Worker(void*);

private:
	bool					SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);
	void					CreateWorld(int& worldBottomRightCorner);

	void					PrintThreadJobsDone();
	void					NewLevel(int level);

	Job*					getJob(int i);

private:
	bool					_running;

	SDL_Window*				_window;
	SDL_Renderer*			_renderer;

	Level*					_level;
	Camera*					_camera;
	Player*					_player;
	Data*					_data;

	vector<SDL_Thread*>		_threads;
	vector<NPC*>			_npcs;

	map<string, int>*		_threadJobDoneLog;

	unsigned int			_lastTime;//time of last update;
	int						_currentLevel;
	bool					_threadIt;
	bool					_hasThreaded;

	static SDL_semaphore*	_semaphore;
	static SDL_mutex*		_mutex;
	static SDL_bool			_canWork;
	static int				_threadsFinished;
};



#endif

