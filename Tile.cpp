#include <Tile.h>

Tile::Tile()
	: _checked(false)
	, _isOccupied(false)
{
	//DEBUG_MSG("Constructing Tile");
}

Tile::~Tile()
{
	//DEBUG_MSG("Destructing Tile");
}

void Tile::Initialize(SDL_Rect rectangle, Type type)
{
	ChangeTile(type);

	GameObject::Initialize(rectangle, _colour);
}
void Tile::ChangeTile(Type type)
{
	_type = type;

	switch (_type)
	{
	case Tile::Type::Empty:
		_isOccupied = false;

		_colour.g = 0;
		_colour.r = 0;
		_colour.b = 0;
		_colour.a = 255;
		break;

	case Tile::Type::Wall:
		_isOccupied = true;

		_colour.g = 100;
		_colour.r = 100;
		_colour.b = 100;
		_colour.a = 255;
		break;

	case Tile::Type::PlayerSpawn:
		_isOccupied = false;

		_colour.g = 125;
		_colour.r = 0;
		_colour.b = 0;
		_colour.a = 255;
		break;

	case Tile::Type::NpcSpawn:
		_isOccupied = false;

		_colour.g = 0;
		_colour.r = 125;
		_colour.b = 0;
		_colour.a = 255;
		break;

	default:
		break;
	}
}

void Tile::Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera)
{
	SDL_SetRenderDrawColor(sdl_renderer, _colour.r, _colour.g, _colour.b, _colour.a);

	SDL_Rect rectangleToRender = SDL_Rect(_rectangle);

	rectangleToRender.x -= camera.x;
	rectangleToRender.y -= camera.y;

	if (_type == Tile::Type::Empty)
	{
		SDL_RenderDrawRect(sdl_renderer, &rectangleToRender);
	}
	else
	{
		SDL_RenderFillRect(sdl_renderer, &rectangleToRender);
	}
}

void Tile::Update()
{
	DEBUG_MSG("Tile Updating");
}

void Tile::CleanUp()
{
	DEBUG_MSG("Cleaning Up Tile");
}

void Tile::SetChecked(bool checked)
{
	_checked = checked;
}
void Tile::SetOccupied(bool isOccupied)
{
	_isOccupied = isOccupied;
}

bool Tile::getChecked() const
{
	return _checked;
}
bool Tile::getOccupied() const
{
	return _isOccupied;
}

Tile::Type Tile::getType() const
{
	return _type;
}