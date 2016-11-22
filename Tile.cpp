#include <Tile.h>

Tile::Tile()
{
	//DEBUG_MSG("Constructing Tile");
}

Tile::~Tile()
{
	DEBUG_MSG("Destructing Tile");
}

void Tile::Initialize(SDL_Rect rectangle, bool isPassible)
{
	//DEBUG_MSG("Tile Initializing");
	SDL_Color colour = SDL_Color();

	_passible = isPassible;

	if (_passible)
	{
		colour.g = 0;
		colour.r = 0;
		colour.b = 0;
		colour.a = 255;
	}
	else
	{
		colour.g = 100;
		colour.r = 100;
		colour.b = 100;
		colour.a = 255;
	}

	GameObject::Initialize(rectangle, colour);
}
void Tile::ChangeTile(bool isPassible)
{
	_passible = isPassible;

	if (_passible)
	{
		_colour.g = 0;
		_colour.r = 0;
		_colour.b = 0;
		_colour.a = 255;
	}
	else
	{
		_colour.g = 100;
		_colour.r = 100;
		_colour.b = 100;
		_colour.a = 255;
	}
}
/*
void Tile::Render()
{
DEBUG_MSG("Tile Drawing");
}
*/
void Tile::Update()
{
	DEBUG_MSG("Tile Updating");
}

void Tile::CleanUp()
{
	DEBUG_MSG("Cleaning Up Tile");
}

bool Tile::GetPassible()
{
	return _passible;
}