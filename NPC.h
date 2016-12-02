#ifndef NPC_H
#define NPC_H

#include "GameObject.h"

class NPC : public GameObject
{
public:
			NPC();
			~NPC();
	//void	Initialize(SDL_Rect rectangle, SDL_Color colour);
	//void Render();
	void	Update() override;
	void	CleanUp() override;

	void	CalculateAstar();

private:
	int		testValue;
};
#endif