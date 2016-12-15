#pragma once

#include "NPC.h"
#include "Grid.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>



class ThreadPool
{
private:
	struct Job
	{
		Job(NPC*& npc, int pX, int pY)
			: _npc(npc)
			, playerX(pX)
			, playerY(pY)
		{

		}

		NPC*& _npc;

		int playerX;
		int playerY;
	};

public:
							ThreadPool();
							~ThreadPool();

	static ThreadPool*		Instance();

	void					Setup(Grid*& grid, int tileSize);
	void					setJob(NPC*& npc, int playerX, int playerY);
	void					PrintThreadJobsDone();
	void					Destroy();

	static int				Worker(void*);

	void					RequestJob(Job*& job);
	Grid*&					getGrid();
	int						getTileSize();

private:
	static ThreadPool*		_instance;

	vector<SDL_Thread*>		_threads;
	map<string, int>*		_threadJobDoneLog;

	queue<Job*>				_jobs;
	Grid*					_grid;

	int						_tileSize;

public:
	static SDL_semaphore*	_semaphore;
	static SDL_mutex*		_jobMutex;
	static SDL_mutex*		_countMutex;
	static SDL_bool			_canWork;
	static int				_threadsFinished;
};

