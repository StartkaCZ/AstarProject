#include <Tile.h>

Tile::Tile()
{
	//DEBUG_MSG("Constructing Tile");
}

Tile::~Tile()
{
	DEBUG_MSG("Destructing Tile");
}

void Tile::Initialize(SDL_Rect rectangle, Type type)
{
	//DEBUG_MSG("Tile Initializing");
	ChangeTile(type);

	GameObject::Initialize(rectangle, _colour);
}
void Tile::ChangeTile(Type type)
{
	_type = type;

	switch (_type)
	{
	case Tile::Type::Empty:
		_colour.g = 0;
		_colour.r = 0;
		_colour.b = 0;
		_colour.a = 255;
		break;

	case Tile::Type::Wall:
		_colour.g = 100;
		_colour.r = 100;
		_colour.b = 100;
		_colour.a = 255;
		break;

	case Tile::Type::PlayerSpawn:
		_colour.g = 255;
		_colour.r = 0;
		_colour.b = 0;
		_colour.a = 255;
		break;

	case Tile::Type::NpcSpawn:
		_colour.g = 0;
		_colour.r = 255;
		_colour.b = 0;
		_colour.a = 255;
		break;

	default:
		break;
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

Tile::Type Tile::getType()
{
	return _type;
}