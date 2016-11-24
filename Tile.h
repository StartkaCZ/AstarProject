#ifndef TILE_H
#define TILE_H

#include "GameObject.h"

class Tile : public GameObject
{
public:
	enum class Type
	{
		Empty,
		Wall,
		PlayerSpawn,
		NpcSpawn
	};


			Tile();
			~Tile();
	void	Initialize(SDL_Rect rectangle, Type type);
	void	ChangeTile(Type type);
	//void Render();
	void	Update() override;
	void	CleanUp() override;
	
	Type	getType();

private:
	Type	_type;
};
#endif