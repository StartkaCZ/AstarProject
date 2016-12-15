#include "Grid.h"

#include <sstream>
#include <assert.h>

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

void Grid::Destroy()
{
	for (int i = 0; i < _tiles.size(); i++)
	{
		for (int j = 0; j < _tiles[i].size(); j++)
		{
			delete _tiles[i][j];
		}

		_tiles[i].clear();
	}

	_tiles.clear();
}

vector<Tile*> Grid::CalculateAstar(int goalX, int goalY, int startX, int startY, int tileSize, string threadName)
{
	TileComparer comparer = TileComparer();
	comparer.threadName = threadName;

	//switch bool _checked into a map of bools
	priority_queue<Tile*, vector<Tile*>, TileComparer> _openList = priority_queue<Tile*, vector<Tile*>, TileComparer>(comparer);
	list<Tile*> _closedList = list<Tile*>();
	vector<Tile*> path = vector<Tile*>();
	
	startX /= tileSize;
	startY /= tileSize;

	goalX /= tileSize;
	goalY /= tileSize;

	_closedList.clear();
	
	Tile* start = _tiles[startX][startY];
	start->getNodeData()[threadName].SetIndex(startX, startY);
	start->getNodeData()[threadName].g = 0;
	start->getNodeData()[threadName].h = 0;
	start->getNodeData()[threadName].f = 0;
	start->getNodeData()[threadName].open = true;

	//CHECK FOR THE CRITICAL SECTION
	/*if (start->getNodeData().size() > 0)
	{
		if (start->getNodeData()[threadName].indexX != -1 || start->getNodeData()[threadName].indexY - 1)
		{
			for (int i = 0; i < start->getNodeData().size(); i++)
			{
				std::ostringstream oss;
				oss << i;

				if (start->getNodeData().find(oss.str()) != start->getNodeData().end())
				{
					if (threadName != oss.str())
					{

						bool equal = start->getNodeData()[threadName].indexX == start->getNodeData()[oss.str()].indexX &&
							start->getNodeData()[threadName].indexY == start->getNodeData()[oss.str()].indexY;

						assert(!equal);

						if (equal)
						{
							DEBUG_MSG("BREACH DETECTED");
						}
					}
				}
			}
		}
	}*/
	

	
	Tile* goal = _tiles[goalX][goalY];
	goal->getNodeData()[threadName].SetIndex(goalX, goalY);

	_openList.push(start);

	DEBUG_MSG("Calculating A*...");

	while (start != goal)
	{
		if (_openList.empty())
		{//If open list is empty then there is no solution
			DEBUG_MSG("No Solution");
			break;
		}

		//Take best node B from open list
		Tile* b = _openList.top();

		int g = 0;
		int h = 0;
		int f = 0;

		vector<Tile*> connections = getConnections(b->getNodeData()[threadName].indexX, b->getNodeData()[threadName].indexY, threadName);
		for (int i = 0; i < connections.size(); i++)
		{
			Tile* n = connections[i];

			while (n->getNodeData()[threadName].indexX == -1 || n->getNodeData()[threadName].indexY == -1)
			{
				connections = getConnections(b->getNodeData()[threadName].indexX, b->getNodeData()[threadName].indexY, threadName);
			}
		}
		
		for (int i = 0; i < connections.size(); i++)
		{//For each node N connected to B
			Tile* n = connections[i];

			if (!n->getNodeData()[threadName].closed)
			{
				g = b->getNodeData()[threadName].g + 1;

				if (n->getNodeData()[threadName].open)
				{//If N is in open list then
					if (g < n->getNodeData()[threadName].g)
					{//If new Path is better then
					 //Replace old entry for N with new one
						n->getNodeData()[threadName].g = g;
						n->getNodeData()[threadName].SetParentIndex(b->getNodeData()[threadName].indexX, b->getNodeData()[threadName].indexY);
					}
				}
				else
				{//Else
					h = abs(goalX - n->getNodeData()[threadName].indexX) + abs(goalY - n->getNodeData()[threadName].indexY);
					f = g + h;

					n->getNodeData()[threadName].g = g;
					n->getNodeData()[threadName].h = h;
					n->getNodeData()[threadName].f = f;

					n->getNodeData()[threadName].SetParentIndex(b->getNodeData()[threadName].indexX, b->getNodeData()[threadName].indexY);

					//Add N to open list
					n->getNodeData()[threadName].open = true;
					_openList.push(n);
				}
			}
		}

		
		if (b == goal)
		{//If b == Goal then solution found
			path.push_back(b);
			_closedList.push_back(b);

			//b->ChangeTile(Tile::Type::Path);

			b->getNodeData()[threadName].SetParentIndex(_openList.top()->getNodeData()[threadName].parentIndexX, _openList.top()->getNodeData()[threadName].parentIndexY);

			int parentX = b->getNodeData()[threadName].parentIndexX;
			int parentY = b->getNodeData()[threadName].parentIndexY;
			while (parentX != -1 && parentY != -1)
			{
				Tile* step = _tiles[parentX][parentY];

				path.push_back(step);
				//step->ChangeTile(Tile::Type::Path);

				
				parentX = step->getNodeData()[threadName].parentIndexX;
				parentY = step->getNodeData()[threadName].parentIndexY;
			}

			std::reverse(path.begin(), path.end());

			break;
		}

		//Add B to Closed List
		_openList.pop();
		_closedList.push_back(b);
		b->getNodeData()[threadName].open = false;
		b->getNodeData()[threadName].closed = true;
		//b->ChangeTile(Tile::Type::ClosedList);
	}
	
	while (_openList.size() != 0)
	{
		_closedList.push_back(_openList.top());
		_openList.pop();
	}

	list<Tile*>::iterator begin = _closedList.begin();
	list<Tile*>::const_iterator end = _closedList.end();

	for (; begin != end; begin++)
	{
		(*begin)->getNodeData()[threadName].Restart();
	}
	
	DEBUG_MSG("... A* Finished");
	
	return path;
}

vector<Tile*> Grid::getConnections(int currentX, int currentY, string threadName)
{
	vector<Tile*> connections;

	int indexer = currentX - 1;
	if (indexer >= 0 && _tiles[indexer][currentY]->getType() != Tile::Type::Wall)
	{
		Tile* connection = _tiles[indexer][currentY];
		
		connection->getNodeData()[threadName].SetIndex(indexer, currentY);

		while (connection->getNodeData()[threadName].indexX == -1 || connection->getNodeData()[threadName].indexY == -1)
		{
			connection->getNodeData()[threadName].SetIndex(indexer, currentY);
		}

		connections.push_back(connection);
	}

	indexer = currentX + 1;
	if (indexer < _tiles.size() && _tiles[indexer][currentY]->getType() != Tile::Type::Wall)
	{
		Tile* connection = _tiles[indexer][currentY];

		connection->getNodeData()[threadName].SetIndex(indexer, currentY);

		while (connection->getNodeData()[threadName].indexX == -1 || connection->getNodeData()[threadName].indexY == -1)
		{
			connection->getNodeData()[threadName].SetIndex(indexer, currentY);
		}

		connections.push_back(connection);
	}

	indexer = currentY - 1;
	if (indexer >= 0 && _tiles[currentX][indexer]->getType() != Tile::Type::Wall)
	{
		Tile* connection = _tiles[currentX][indexer];

		connection->getNodeData()[threadName].SetIndex(currentX, indexer);

		while (connection->getNodeData()[threadName].indexX == -1 || connection->getNodeData()[threadName].indexY == -1)
		{
			connection->getNodeData()[threadName].SetIndex(currentX, indexer);
		}

		connections.push_back(connection);
	}

	indexer = currentY + 1;
	if (indexer < _tiles[currentX].size() && _tiles[currentX][indexer]->getType() != Tile::Type::Wall)
	{
		Tile* connection = _tiles[currentX][indexer];

		connection->getNodeData()[threadName].SetIndex(currentX, indexer);

		while (connection->getNodeData()[threadName].indexX == -1 || connection->getNodeData()[threadName].indexY == -1)
		{
			connection->getNodeData()[threadName].SetIndex(currentX, indexer);
		}

		connections.push_back(connection);
	}

	return connections;
}

vector<vector<Tile*>>& Grid::getTiles()
{
	return _tiles;
}
