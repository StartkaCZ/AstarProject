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

class Game
{
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

	static void				P(); //used to delay process
	static void				V(); //used to signal occurance
	static int				Worker(void*);

private:
	bool					_running;

	SDL_Window*				_window;
	SDL_Renderer*			_renderer;

	Level*					_level;
	Camera*					_camera;
	Player*					_player;
	vector<NPC*>			_npcs;

	unsigned int			_lastTime;//time of last update;
	int						_currentLevel;

	int						_arrive[8] = { 0 };
	int						_continue[8] = { 0 };

	static Grid*			_Grid;
	static queue<NPC*>		_jobs;
	static int				_mutex;
	static bool				_lock;
};



#endif

