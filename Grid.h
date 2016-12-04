#pragma once

#include "Tile.h"

#include <queue>
#include <list>
#include <vector>

class Grid
{
public:
	Grid();
	~Grid();

	void						CalculateAstar();

	void						Update();
	void						Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera, int tileSize);
	void						TakeTile(SDL_Rect takenRectangle);

	vector<vector<Tile*>>&		getTiles();


private:
	vector<vector<Tile*>>		_tiles;

	list<Tile>					_closedList;
	priority_queue<Tile>		_openList;
};