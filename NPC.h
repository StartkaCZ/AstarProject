#ifndef NPC_H
#define NPC_H

#include "GameObject.h"
#include<queue>
#include<list>

class NPC : public GameObject
{
private:
	struct Node
	{
		int		f;
		int		g;
		int		h;

		Node*	parent;
	};

public:
			NPC();
			~NPC();
	//void	Initialize(SDL_Rect rectangle, SDL_Color colour);
	//void Render();
	void	Update() override;
	void	CleanUp() override;

	void	CalculateAstar();

private:
	
	std::list<Node>				_closedList;
	std::priority_queue<Node>	_openList;
};
#endif