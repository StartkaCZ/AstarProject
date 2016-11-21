#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
public:
			Player();
			~Player();
	//void Initialize(SDL_Rect rectangle, SDL_Color colour);
	//void Render();
	void	Update() override;
	void	CleanUp() override;
};
#endif
