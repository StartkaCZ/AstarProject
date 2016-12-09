#include "Grid.h"

Grid::Grid()
	: _closedList(list<Tile>())
	, _openList(priority_queue<Tile>())
	, _regions(vector<Region*>())
	, _waypoints(vector<WayPoint*>())
{
	
}

//implement regions
//impelent waypoints
//implement A*

Grid::~Grid()
{
	DEBUG_MSG("Destructing Grid");
}

void Grid::Optimize(int maxWalls)
{
	SetupWayPoints(maxWalls);
	SetupRegions(maxWalls);
}

void Grid::SetupWayPoints(int maxWalls)
{
	for (int i = 0; i < _tiles.size(); i++)
	{
		for (int i = 0; i < _tiles[i].size(); i++)
		{

		}
	}
}

void Grid::SetupRegions(int maxWalls)
{
	int startingIndex = 0;
	int regions = maxWalls;

	//From begining till the last wall
	while (_regions.size() < regions)
	{
		Region* region = new Region();
		region->fromIndex = 0;

		for (int i = startingIndex + 1; i < _tiles.size(); i++)
		{
			if (_tiles[i][1]->getType() == Tile::Type::Wall)
			{
				region->toIndex = startingIndex = i;
			}
		}

		region->containsPlayer = false;

		_regions.push_back(region);
	}

	//Last Region
	Region* region = new Region();

	region->fromIndex = startingIndex;
	region->toIndex = _tiles[0].size();
	region->containsPlayer = false;

	_regions.push_back(region);
}

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