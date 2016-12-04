#ifndef NPC_H
#define NPC_H

#include "GameObject.h"

class NPC : public GameObject
{
public:
			NPC();
			~NPC();

	void	Update() override;
	void	CleanUp() override;
};
#endif