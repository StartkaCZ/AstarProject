#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "EnumHolder.h"
#include "Tile.h"

#include <vector>

class Player : public GameObject
{
public:
				Player();
				~Player();

	void		Initialize(SDL_Rect rectangle, SDL_Color colour, float interpolationTimer);

	void		Update() override;
	void		Update(vector<vector<Tile*>>& tiles, int tileSize, int dt);

	void		CleanUp() override;

private:
	void		Wonder(vector<vector<Tile*>>& tiles, int tileSize);
	void		Move(Tile* tile, bool& canMoveToTile, Direction randomDirection);

	void		Interpolate();

private:
	Direction	_lastDirectionMoved;

	float		_wonderTimer;
	float		_maxWonderTimer;

	int			_initialX;
	int			_initialY;
	int			_goalX;
	int			_goalY;
};
#endif
