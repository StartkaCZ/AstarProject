#include<Level.h>


Level::Level(int level)
{
	if (level == 0)
	{
		_worldScale = 1.0f;
		_maxRowCol = 30;
		_maxNPC = 5;
		_maxWalls = 3;
	}
	else if (level == 1)
	{
		_worldScale = 1.0f;
		_maxRowCol = 100;
		_maxNPC = 50;
		_maxWalls = 6;
	}
	else if (level == 2)
	{
		_worldScale = 5.0f;
		_maxRowCol = 1000;
		_maxNPC = 500;
		_maxWalls = 18;
	}
}
Level::~Level()
{

}

void Level::Initialize(Player*& player, vector<NPC*>& npcs, vector<vector<Tile*>>& tiles, int& worldBottomRightCorner, int width, int height)
{
	//tiles available = row/col max tiles - (edge + edge) - (spacing for player/NPC area)
	int spawnAreaTiles = _maxRowCol * 0.2f;
	int tilesAvailble = _maxRowCol - 2 - spawnAreaTiles - _maxWalls;
	int spacing = tilesAvailble / (_maxWalls - 1);
	int offset = 2 + spawnAreaTiles * 0.5f;

	SetupTiles(tiles, worldBottomRightCorner, width, height);
	SetupWalls(tiles, offset, spacing);
	SetupPlayerSpawnArea(player, tiles, offset);
	SetupNPC_SpawnArea(npcs, tiles, offset, spacing);
}

void Level::SetupTiles(vector<vector<Tile*>>& tiles, int& worldBottomRightCorner, int width, int height)
{
	//tile size = ((width + height) * 0.5 * 4) / maxRowCol
	// scale the world 4 times
	_tileSize = ((width + height) * 0.5f * _worldScale) / _maxRowCol;
	worldBottomRightCorner = _tileSize * _maxRowCol;

	SDL_Rect setupRectangle = SDL_Rect();
	SDL_Color setupColour = SDL_Color();

	int x = 0;
	int y = 0;

	for (int i = 0; i < _maxRowCol; i++)
	{
		tiles.push_back(vector<Tile*>());

		for (int j = 0; j < _maxRowCol; j++)
		{
			Tile::Type type = Tile::Type::Empty;

			setupRectangle.x = x;
			setupRectangle.y = y;
			setupRectangle.h = _tileSize;
			setupRectangle.w = _tileSize;

			tiles[i].push_back(new Tile());

			if (i == 0 || j == 0 || i == _maxRowCol - 1 || j == _maxRowCol - 1)
			{
				type = Tile::Type::Wall;
			}

			tiles[i][j]->Initialize(setupRectangle, type);
			y += _tileSize;
		}

		y = 0;
		x += _tileSize;
	}
}
void Level::SetupWalls(vector<vector<Tile*>>& tiles, int offset, int spacing)
{
	int tilesToWalls = (_maxRowCol - 2) * 0.9f;
	bool bottomToTop = false;

	for (int k = 0; k < _maxWalls; k++)
	{
		int i = 0;

		if (k == 0)
		{
			int halfSpacing = spacing*0.5f;
			int randomOffset = rand() % halfSpacing;
			i = offset + randomOffset + (spacing * k);
		}
		else if (k != _maxWalls - 1)
		{
			int halfSpacing = spacing*0.5f;
			int randomOffset = rand() % halfSpacing - (halfSpacing*0.5f);
			i = offset + randomOffset + (spacing * k);
		}
		else
		{
			int halfSpacing = spacing*0.5f;
			int randomOffset = rand() % halfSpacing - (spacing*0.75f);
			i = offset + randomOffset + (spacing * k);
		}

		if (bottomToTop)
		{
			for (int j = 1; j < tilesToWalls; j++)
			{
				tiles[i][j]->ChangeTile(Tile::Type::Wall);
			}
		}
		else
		{
			int amount = _maxRowCol - tilesToWalls - 2;

			for (int j = _maxRowCol - 1; j > amount; j--)
			{
				tiles[i][j]->ChangeTile(Tile::Type::Wall);
			}
		}

		bottomToTop = !bottomToTop;
	}
}

void Level::SetupPlayerSpawnArea(Player*& player, vector<vector<Tile*>>& tiles, int offset)
{
	int rowLimit = offset;
	int colLimit = _maxRowCol - rowLimit - 1;
	int tileCounter = 0;

	for (int i = 1; i < rowLimit; i++)
	{
		for (int j = _maxRowCol - 2; j > colLimit; j--)
		{
			tiles[i][j]->ChangeTile(Tile::Type::PlayerSpawn);

			if (player == nullptr)
			{
				SpawnPlayer(player, tiles[i][j]);
			}

			tileCounter++;
		}
	}
	string message = "" + tileCounter;
	DEBUG_MSG("Player Spawn Tiles: ");
	DEBUG_MSG(tileCounter);
}
void Level::SetupNPC_SpawnArea(vector<NPC*>& npcs, vector<vector<Tile*>>& tiles, int offset, int spacing)
{
	int rowLimit = offset + (spacing * (_maxWalls - 1));
	int colLimit = _maxRowCol / 3;
	int tileCounter = 0;

	for (int i = _maxRowCol - 2; i > rowLimit; i--)
	{
		for (int j = _maxRowCol * 2 / 3; j > colLimit; j--)
		{
			tiles[i][j]->ChangeTile(Tile::Type::NpcSpawn);

			if (npcs.size() < _maxNPC)
			{
				SpawnNPC(npcs, tiles[i][j]);
			}

			tileCounter++;
		}
	}

	std::reverse(npcs.begin(), npcs.end());

	DEBUG_MSG("NPC Spawn Tiles: ");
	DEBUG_MSG(tileCounter);
}

void Level::SpawnPlayer(Player*& player, Tile*& tile)
{
	if (!tile->getOccupied())
	{
		SDL_Rect rectangle = CreateRectangle(tile);
		SDL_Color colour = CreateColour(tile, true);

		player = new Player();
		player->Initialize(rectangle, colour);
		tile->SetOccupied(true);
	}
}
void Level::SpawnNPC(vector<NPC*>& npcs, Tile*& tile)
{
	if (!tile->getOccupied())
	{
		SDL_Rect rectangle = CreateRectangle(tile);
		SDL_Color colour = CreateColour(tile, false);

		npcs.push_back(new NPC());
		npcs[npcs.size() - 1]->Initialize(rectangle, colour);
		tile->SetOccupied(true);
	}
}

SDL_Rect Level::CreateRectangle(const Tile* tile)
{
	SDL_Rect rectangle = SDL_Rect();

	int halfWidth = tile->getRectangle().w * 0.5f;
	int halfHeight = tile->getRectangle().h * 0.5f;

	rectangle.x = tile->getRectangle().x + (halfWidth * 0.5f);
	rectangle.y = tile->getRectangle().y + (halfHeight * 0.5f);
	rectangle.w = halfWidth;
	rectangle.h = halfHeight;

	return rectangle;
}
SDL_Color Level::CreateColour(const Tile* tile, bool isPlayer)
{
	SDL_Color colour = SDL_Color();

	if (isPlayer)
	{
		colour.r = 0;
		colour.g = 255;
		colour.b = 0;
		colour.a = 255;
	}
	else
	{
		colour.r = 255;
		colour.g = 0;
		colour.b = 0;
		colour.a = 255;
	}

	return colour;
}

int	Level::getTileSize() const
{
	return _tileSize;
}

int Level::getMaxWalls() const
{
	return _maxWalls;
}