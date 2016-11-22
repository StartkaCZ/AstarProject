#ifndef GAME_H
#define GAME_H

#include "Debug.h"
#include "Player.h"
#include "Tile.h"
#include "NPC.h"

#include <SDL.h>
#include <vector>

class Game
{
public:
							Game();
							~Game();

	bool					Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	bool					SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);
	void					SetupLevel(int width, int height);

	void					LoadContent();
	void					UnloadContent();
	void					Render();
	void					Update();
	void					HandleEvents();
	bool					IsRunning();
	void					CleanUp();

private:
	bool					_running;

	SDL_Window*				_window;
	SDL_Renderer*			_renderer;

	Player*					_player;
	vector<NPC*>			_npcs;
	vector<vector<Tile*>>	_tiles;

	SDL_Rect				_playerSpawnArea;
	SDL_Rect				_NPCSpawnArea;
	SDL_Rect				_camera;

	int						_level;
	int						_maxRowCol;
	int						_maxNPC;

	int						_worldBottomRightCorner;
	float					_worldScale;
};
#endif

