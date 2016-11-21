#ifndef TILE_H
#define TILE_H

#include "GameObject.h"

class Tile : public GameObject
{
public:
			Tile();
			~Tile();
	void	Initialize(SDL_Rect rectangle, bool isPassible);
	//void Render();
	void	Update() override;
	void	CleanUp() override;
	
	bool	GetPassible();

private:
	bool	_passible;
};
#endif