#include <NPC.h>

NPC::NPC()
	: _path(vector<Tile*>())
	, _pathComplete(true)
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
	if (_path.size() == 1)
	{
		_pathComplete = true;
	}

	if (_interpolationTimer > _maxInterpolationTimer)
	{
		if (!_pathComplete && !_path[1]->getOccupied())
		{
			_interpolationTimer = 0;

			_rectangle.x = _goalX;
			_rectangle.y = _goalY;

			_path[0]->SetOccupied(false);
			_path.erase(_path.begin());
			Move();
		}
	}
	else
	{
		_interpolationTimer += dt / 1000.f;
		Interpolate();
	}
}
void NPC::Interpolate()
{
	float scale = _interpolationTimer / _maxInterpolationTimer;

	_rectangle.x = _initialX + (_goalX - _initialX) * scale;
	_rectangle.y = _initialY + (_goalY - _initialY) * scale;
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

void NPC::SetPath(vector<Tile*> newPath)
{
	if (newPath.size() > 0)
	{
		_pathComplete = false;
		_path = newPath;
	}
}

void NPC::CleanUp()
{
	DEBUG_MSG("Cleaning Up NPC");
}