#include "Grid.h"

Grid::Grid()
	: _closedList(list<Tile>())
	, _openList(priority_queue<Tile>())
{
	
}

Grid::~Grid()
{
	DEBUG_MSG("Destructing Grid");
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

void Grid::CalculateAstar()
{
	DEBUG_MSG("Calculating A*...");

	_closedList.clear();
	//_openList.push;

	DEBUG_MSG("... A* Finished");
}

void Grid::TakeTile(SDL_Rect takenRectangle)
{

}

vector<vector<Tile*>>& Grid::getTiles()
{
	return _tiles;
}