#pragma once

#include "Player.h"
#include "Tile.h"
#include "NPC.h"

#include <vector>

class Level
{
public:
							Level(int level);
							~Level();
	
	void					Initialize(Player*& player, vector<NPC*>& npcs, vector<vector<Tile*>>& tiles, int& worldBottomRightCorner, int width, int height);

	void					SetupTiles(vector<vector<Tile*>>& tiles, int& worldBottomRightCorner, int width, int height);
	void					SetupWalls(vector<vector<Tile*>>& tiles, int offset, int spacing);
	void					SetupPlayerSpawnArea(Player*& player, vector<vector<Tile*>>& tiles, int offset);
	void					SpawnPlayer(Player*& player, Tile *& tile);
	void					SetupNPC_SpawnArea(vector<NPC*>& npcs, vector<vector<Tile*>>& tiles, int offset, int spacing);
	void					SpawnNPC(vector<NPC*>& npcs, Tile*& tile);

	SDL_Rect				CreateRectangle(const Tile* tile);
	SDL_Color				CreateColour(const Tile* tile, bool isPlayer);

	int						getTileSize() const;
	int						getMaxWalls() const;

private:
	int						_maxRowCol;
	int						_maxNPC;
	int						_maxWalls;

	int						_tileSize;
	float					_worldScale;
	float					_interpolationTimer;
};