#pragma once

#include "Tile.h"

#include <queue>
#include <list>
#include <vector>

class Grid
{
	struct WayPoint
	{
		int row;
		int col;

		int nextRow;
		int nextCol;
	};

	struct Region
	{
		int		fromIndex;
		int		toIndex;

		bool	containsPlayer;
	};

public:
	Grid();
	~Grid();

	void						Optimize(int maxWalls);

	void						Update();
	void						Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera, int tileSize);

	vector<Tile*>				CalculateAstar();
	vector<vector<Tile*>>&		getTiles();

private:
	void						SetupRegions(int maxWalls);
	void						SetupWayPoints(int maxWalls);

private:
	vector<vector<Tile*>>		_tiles;
	vector<WayPoint*>			_waypoints;
	vector<Region*>				_regions;

	list<Tile>					_closedList;
	priority_queue<Tile>		_openList;
};