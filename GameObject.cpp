#include <GameObject.h>

GameObject::GameObject()
{
	//DEBUG_MSG("Character Constructor");
}

GameObject::~GameObject()
{
	DEBUG_MSG("Character Destructor");
}

void GameObject::Initialize(SDL_Rect rectangle, SDL_Color colour)
{
	_rectangle = rectangle;
	_colour = colour;
}
void GameObject::Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera)
{
	SDL_SetRenderDrawColor(sdl_renderer, _colour.r, _colour.g, _colour.b, _colour.a);

	SDL_Rect rectangleToRender = SDL_Rect(_rectangle);

	rectangleToRender.x -= camera.x;
	rectangleToRender.y -= camera.y;

	SDL_RenderFillRect(sdl_renderer, &rectangleToRender);
}

SDL_Rect GameObject::getRectangle() const
{
	return _rectangle;
}
