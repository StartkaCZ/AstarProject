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

	

	struct TileComparer
	{
		string threadName;

		bool operator()(Tile* lhs, Tile* rhs)
		{//from lowest to highest
			return lhs->getNodeData()[threadName].f > rhs->getNodeData()[threadName].f;
		}
	};

public:
	Grid();
	~Grid();

	//void						Optimize(int maxWalls);

	void						Update();
	void						Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera, int tileSize);

	void						Destroy();

	vector<Tile*>				CalculateAstar(int goalX, int goalY, int startX, int startY, int tileSize, string threadName);
	vector<vector<Tile*>>&		getTiles();

	vector<Tile*>				getConnections(int currentX, int currentY, string threadName);

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