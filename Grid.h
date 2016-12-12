#pragma once

#include "Tile.h"

#include <queue>
#include <list>
#include <vector>

class Grid
{
	/*struct WayPoint
	{
		int row;
		int col;

		int nextRow;
		int nextCol;
	};

	struct Region
	{
		int		fromIndexX;
		int		toIndexX;

		bool	containsPlayer;

		vector<WayPoint> wayPoints;
	};*/

public:
	Grid();
	~Grid();

	//void						Optimize(int maxWalls);

	void						Update();
	void						Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera, int tileSize);

	vector<Tile*>				CalculateAstar();
	vector<vector<Tile*>>&		getTiles();

/*
private:
	void						SetupRegions(int maxWalls);
	void						SetupWayPoints(int maxWalls);

	void						WallBottomToTop(int indexerX, int wayPointCount);
	void						WallTopToBottom(int indexerX, int wayPointCount);
*/
private:
	vector<vector<Tile*>>		_tiles;
	//vector<WayPoint*>			_waypoints;
	//vector<Region*>			_regions;
};