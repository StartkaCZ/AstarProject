#include <Tile.h>

Tile::Tile()
{
	DEBUG_MSG("Constructing Tile");
}

Tile::~Tile()
{
	DEBUG_MSG("Destructing Tile");
}
/*
void Tile::Initialize(SDL_Rect rectangle, SDL_Color colour)
{
DEBUG_MSG("Tile Initializing");

}

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