#include <NPC.h>

NPC::NPC()
	: _path(vector<Tile*>())
	, _pathComplete(true)
	, _requestPath(true)
	, _playerReached(false)
	, _initialPathSize(0)
{ 
	
}

NPC::~NPC()
{
	//DEBUG_MSG("Destructing NPC");
}

void NPC::Initialize(SDL_Rect rectangle, SDL_Color colour, float interpolationTimer)
{
	_rectangle = rectangle;
	_colour = colour;

	_initialX = _rectangle.x;
	_initialY = _rectangle.y;

	_goalX = _initialX;
	_goalY = _initialY;

	_maxInterpolationTimer = interpolationTimer;
}

void NPC::Update()
{
	//DEBUG_MSG("NPC Updating");
}

void NPC::Update(int tileSize, int dt)
{
	if (_interpolationTimer > _maxInterpolationTimer)
	{
		CheckForPath();
	}
	else
	{
		_interpolationTimer += dt / 1000.f;
		Interpolate();
	}
}
void NPC::CheckForPath()
{
	if (!_pathComplete && !_path[1]->getOccupied())
	{
		_interpolationTimer = 0;

		_rectangle.x = _goalX;
		_rectangle.y = _goalY;

		_path[0]->SetOccupied(false);
		_path.erase(_path.begin());

		if (_path.size() == 1)
		{
			_playerReached = true;
			_pathComplete = true;
		}
		else if (_path.size() < _initialPathSize * 0.5f)
		{
			_requestPath = true;
			_initialPathSize = _path.size();
		}

		Move();
	}
	else if (_playerReached)
	{
		_path[0]->SetOccupied(false);
	}
}

void NPC::Move()
{
	int halfWidth = _path[0]->getRectangle().w * 0.5f;
	int halfHeight = _path[0]->getRectangle().h * 0.5f;

	_goalX = _path[0]->getRectangle().x + (halfWidth * 0.5f);
	_goalY = _path[0]->getRectangle().y + (halfHeight * 0.5f);

	_initialX = _rectangle.x;
	_initialY = _rectangle.y;

	_path[0]->SetOccupied(true);
}

void NPC::Interpolate()
{
	float scale = _interpolationTimer / _maxInterpolationTimer;

	_rectangle.x = _initialX + (_goalX - _initialX) * scale;
	_rectangle.y = _initialY + (_goalY - _initialY) * scale;
}

void NPC::SetPath(vector<Tile*> newPath)
{
	if (newPath.size() > 0)
	{
		if (!_pathComplete)
		{
			Tile* currentTile = nullptr;

			for (int i = 0; i < _path.size(); i++)
			{
				if (_path[i]->getOccupied())
				{
					currentTile = _path[i];
					currentTile->SetOccupied(false);
					break;
				}
			}

			vector<Tile*>::iterator iterator = find(newPath.begin(), newPath.end(), currentTile);

			if (iterator != newPath.end())
			{
				newPath.erase(newPath.begin(), iterator);
			}
		}

		_initialPathSize = newPath.size();
		_pathComplete = false;
		_path = newPath;

		Move();
	}
}

bool NPC::IsPathComplete()
{
	if (_requestPath && !_playerReached)
	{
		_requestPath = false;
		return true;
	}

	return false;
}

bool NPC::HasReachedPlayer()
{
	return _playerReached;
}

void NPC::CleanUp()
{
	DEBUG_MSG("Cleaning Up NPC");
}