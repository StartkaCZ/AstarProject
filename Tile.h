#ifndef TILE_H
#define TILE_H

#include "GameObject.h"

class Tile : public GameObject
{
public:
			Tile();
			~Tile();
	//void	Initialize(SDL_Rect rectangle, SDL_Color colour);
	//void Render();
	void Update() override;
	void CleanUp() override;
};
#endif