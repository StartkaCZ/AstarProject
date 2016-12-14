#ifndef NPC_H
#define NPC_H

#include "GameObject.h"
#include "Tile.h"

#include <vector>

class NPC : public GameObject
{
public:
				NPC();
				~NPC();

	void		Initialize(SDL_Rect rectangle, SDL_Color colour, float interpolationTimer);

	void		Update() override;
	void		Update(int tileSize, int dt);

	void		SetPath(vector<Tile*> newPath);

	void		CleanUp() override;

	bool		IsPathComplete();

private:
	void		Move();

	void		Interpolate();

private:
	float		_interpolationTimer;
	float		_maxInterpolationTimer;

	int			_initialX;
	int			_initialY;
	int			_goalX;
	int			_goalY;

	vector<Tile*> _path;

	bool		_pathComplete;
};
#endif