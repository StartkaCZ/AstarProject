#include "Camera.h"

Camera::Camera()
{

}
Camera::~Camera()
{

}

void Camera::Initialize(SDL_Rect rectangle, int worldBottomRightCorner)
{
	_rectangle = rectangle;
	_worldBottomRightCorner = worldBottomRightCorner;
}
void Camera::ReInitialize(int worldBottomRightCorner)
{
	_worldBottomRightCorner = worldBottomRightCorner;
}

void Camera::Move(Direction direction)
{
	switch (direction)
	{
	case Direction::Up:
		MoveUp();
		break;
	case Direction::Down:
		MoveDown();
		break;
	case Direction::Right:
		MoveRight();
		break;
	case Direction::Left:
		MoveLeft();
		break;
	default:
		break;
	}
}

void Camera::MoveUp()
{
	if (_rectangle.y > 0)
	{
		_rectangle.y -= 50;
	}
}
void Camera::MoveDown()
{
	if (_rectangle.y + _rectangle.h < _worldBottomRightCorner)
	{
		_rectangle.y += 50;
	}
}
void Camera::MoveRight()
{
	if (_rectangle.x + _rectangle.w < _worldBottomRightCorner)
	{
		_rectangle.x += 50;
	}
}
void Camera::MoveLeft()
{
	if (_rectangle.x > 0)
	{
		_rectangle.x -= 50;
	}
}

SDL_Rect Camera::getRectangle() const
{
	return _rectangle;
}