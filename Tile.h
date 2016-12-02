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
	void	Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera) override;
	void	Update() override;
	void	CleanUp() override;
	
	void	SetOccupied(bool isOccupied);

	Type	getType() const;
	bool	getOccupied() const;

private:
	Type	_type;
	bool	_isOccupied;
};
#endif