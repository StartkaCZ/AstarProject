#pragma once

#include "SDL_rect.h"
#include "EnumHolder.h"

class Camera
{
public:
				Camera();
				~Camera();

	void		Initialize(SDL_Rect rectangle, int worldBottomRightCorner);
	void		ReInitialize(int worldBottomRightCorner);
	void		Move(Direction direction);

	SDL_Rect	getRectangle() const;

private:
	void		MoveUp();
	void		MoveDown();
	void		MoveRight();
	void		MoveLeft();

private:
	SDL_Rect	_rectangle;
	
	int			_worldBottomRightCorner;

	const float	SPEED = 100;
};