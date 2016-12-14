#ifndef GAME_H
#define GAME_H

#include "Debug.h"
#include "Player.h"
#include "Tile.h"
#include "NPC.h"
#include "Level.h"
#include "Camera.h"
#include "Grid.h"
#include "ThreadPool.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>

class Game
{
public:
							Game();
							~Game();

	bool					Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	
	void					Render();
	void					Update();
	void					HandleEvents();
	void					CleanUp();
	bool					IsRunning();

private:
	bool					SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);
	void					CreateWorld(int& worldBottomRightCorner);

	void					NewLevel(int level);

	void					FreeMemory();

private:
	bool					_running;

	SDL_Window*				_window;
	SDL_Renderer*			_renderer;

	Grid*					_grid;
	Level*					_level;
	Camera*					_camera;
	Player*					_player;

	vector<NPC*>			_npcs;

	unsigned int			_lastTime;//time of last update;
	int						_currentLevel;
	bool					_threadIt;
	bool					_hasThreaded;
};



#endif

