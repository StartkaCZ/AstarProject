#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Debug.h"


using namespace std;

class GameObject
{
public:
					GameObject();
					~GameObject();

	virtual void	Initialize(SDL_Rect rectangle, SDL_Color colour);
	virtual void	Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera);
	virtual void	Update() = 0;
	virtual void	CleanUp() = 0;
	

protected:
	SDL_Rect		_rectangle;
	SDL_Color		_colour;
};
#endif
