#include "Grid.h"

Grid::Grid()
	//: _regions(vector<Region*>())
	//, _waypoints(vector<WayPoint*>())
{
	
}


Grid::~Grid()
{
	DEBUG_MSG("Destructing Grid");
}
/*
void Grid::Optimize(int maxWalls)
{
	SetupWayPoints(maxWalls);
	SetupRegions(maxWalls);
}

void Grid::SetupWayPoints(int maxWalls)
{
	int wayPointCount = maxWalls * 2;
	int indexerX = 1;
	bool bottomToTop = false;

	while (_waypoints.size() < wayPointCount)
	{
		if (bottomToTop)
		{
			WallBottomToTop(indexerX, wayPointCount);
			bottomToTop = true;
		}
		else
		{
			WallTopToBottom(indexerX, wayPointCount);
			bottomToTop = false;
		}
	}
}

void Grid::WallBottomToTop(int indexerX, int wayPointCount)
{
	for (int i = indexerX; i < _tiles.size(); i++)
	{
		if (_tiles[i][1]->getType() == Tile::Type::Wall)
		{
			int indexerY = 1;

			for (int j = 1; j < _tiles[i].size(); j++)
			{
				if (_tiles[i][j]->getType() == Tile::Type::Empty)
				{
					indexerY = j;
					break;
				}
			}

			int centerYspace = indexerY + (_tiles[i].size() - 1 - indexerY) * 0.5f;
			int centerXspace = (i - indexerX) * 0.5f;

			WayPoint* wayPoint = new WayPoint();
			wayPoint->row = centerXspace;
			wayPoint->col = centerYspace;

			_waypoints.push_back(wayPoint);
		}
	}
}
void Grid::WallTopToBottom(int indexerX, int wayPointCount)
{

}

void Grid::SetupRegions(int maxWalls)
{
	int startingIndex = 0;
	int regions = maxWalls;

	//From begining till the last wall
	while (_regions.size() < regions)
	{
		Region* region = new Region();
		region->fromIndexX = 0;

		for (int i = startingIndex + 1; i < _tiles.size(); i++)
		{
			if (_tiles[i][1]->getType() == Tile::Type::Wall)
			{
				region->toIndexX = startingIndex = i;
			}
		}

		region->containsPlayer = false;

		_regions.push_back(region);
	}

	//Last Region
	Region* region = new Region();

	region->fromIndexX = startingIndex;
	region->toIndexX = _tiles[0].size();
	region->containsPlayer = false;

	_regions.push_back(region);
}
*/
void Grid::Update()
{
	
}

void Grid::Render(SDL_Renderer*& renderer, const SDL_Rect& cameraRectangle, int tileSize)
{
	int cameraLeftPosition = cameraRectangle.x / tileSize;
	int cameraBottomPosition = cameraRectangle.y / tileSize;
	int cameraRightPosition = (cameraRectangle.x + cameraRectangle.w) / tileSize;
	int cameraTopPosition = (cameraRectangle.y + cameraRectangle.h) / tileSize;


	for (int i = cameraLeftPosition; i < cameraRightPosition; i++)
	{
		for (int j = cameraBottomPosition; j < cameraTopPosition; j++)
		{
			_tiles[i][j]->Render(renderer, cameraRectangle);
		}
	}
}

vector<Tile*> Grid::CalculateAstar()
{
	list<Tile>					_closedList;
	priority_queue<Tile>		_openList;

	vector<Tile*> path = vector<Tile*>();
	DEBUG_MSG("Calculating A*...");

	_closedList.clear();
	//_openList.push;

	DEBUG_MSG("... A* Finished");
	return path;
}

vector<vector<Tile*>>& Grid::getTiles()
{
	return _tiles;
}