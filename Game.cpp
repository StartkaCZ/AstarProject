#include <Game.h>
#include <iostream>
#include <thread>


using namespace std;

Game::Game() : _running(false)
{
}

Game::~Game()
{
}

bool Game::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	_running = SetupSDL(title, xpos, ypos, width, height, flags);
	
	if (_running)
	{
		_level = 0;
		DEBUG_MSG("LEVEL: ");
		DEBUG_MSG(_level);

		_camera.x = 0;
		_camera.y = 0;
		_camera.w = width;
		_camera.h = height;

		SetupLevel();
		SetupWorld(width, height);
	}

	return _running;
}
bool Game::SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		DEBUG_MSG("SDL Init success");
		_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (_window != 0)
		{
			DEBUG_MSG("Window creation success");
			_renderer = SDL_CreateRenderer(_window, -1, 0);
			if (_renderer != 0)
			{
				DEBUG_MSG("Renderer creation success");
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			}
			else
			{
				DEBUG_MSG("Renderer init fail");
				return false;
			}
		}
		else
		{
			DEBUG_MSG("Window init fail");
			return false;
		}
	}
	else
	{
		DEBUG_MSG("SDL init fail");
		return false;
	}

	return true;
}
void Game::SetupLevel()
{
	if (_level == 0)
	{
		_worldScale = 1.0f;
		_maxRowCol = 30;
		_maxNPC = 5;
		_maxWalls = 3;
	}
	else if (_level == 1)
	{
		_worldScale = 1.0f;
		_maxRowCol = 100;
		_maxNPC = 50;
		_maxWalls = 6;
	}
	else if (_level == 2)
	{
		_worldScale = 5.0f;
		_maxRowCol = 1000;
		_maxNPC = 500;
		_maxWalls = 18;
	}
}
void Game::SetupWorld(int width, int height)
{
	_npcs = vector<NPC*>();
	_tiles = vector<vector<Tile*>>(_maxRowCol);
	_renderTiles = vector<Tile*>();

	//tiles available = row/col max tiles - (edge + edge) - (spacing for player/NPC area)
	int spawnAreaTiles = _maxRowCol * 0.2f;
	int tilesAvailble = _maxRowCol - 2 - spawnAreaTiles - _maxWalls;
	int spacing = tilesAvailble / (_maxWalls - 1);
	int offset = 2 + spawnAreaTiles * 0.5f;

	SetupTiles(width, height);
	SetupWalls(offset, spacing);
	SetupPlayerSpawnArea(offset);
	SetupNPC_SpawnArea(offset, spacing);

	_renderTiles.shrink_to_fit();
}

void Game::SetupTiles(int width, int height)
{
	//tile size = ((width + height) * 0.5 * 4) / maxRowCol
	// scale the world 4 times
	int tileSize = ((width + height) * 0.5f * _worldScale) / _maxRowCol;
	_worldBottomRightCorner = tileSize * _maxRowCol;

	SDL_Rect setupRectangle = SDL_Rect();
	SDL_Color setupColour = SDL_Color();

	int x = 0;
	int y = 0;

	for (int i = 0; i < _maxRowCol; i++)
	{
		for (int j = 0; j < _maxRowCol; j++)
		{
			Tile::Type type = Tile::Type::Empty;

			setupRectangle.x = x;
			setupRectangle.y = y;
			setupRectangle.h = tileSize;
			setupRectangle.w = tileSize;

			_tiles[i].push_back(new Tile());

			if (i == 0 || j == 0 || i == _maxRowCol - 1 || j == _maxRowCol - 1)
			{
				type = Tile::Type::Wall;
				_renderTiles.push_back(_tiles[i][j]);
			}

			_tiles[i][j]->Initialize(setupRectangle, type);
			y += tileSize;
		}

		y = 0;
		x += tileSize;
	}
}
void Game::SetupWalls(int offset, int spacing)
{
	int tilesToWalls = (_maxRowCol - 2) * 0.9f;
	bool bottomToTop = false;

	for (int k = 0; k < _maxWalls; k++)
	{
		int i = offset + (spacing * k);

		if (bottomToTop)
		{
			for (int j = 1; j < tilesToWalls; j++)
			{
				_tiles[i][j]->ChangeTile(Tile::Type::Wall);
				_renderTiles.push_back(_tiles[i][j]);
			}
		}
		else
		{
			int amount = _maxRowCol - tilesToWalls - 2;

			for (int j = _maxRowCol - 1; j > amount; j--)
			{
				_tiles[i][j]->ChangeTile(Tile::Type::Wall);
				_renderTiles.push_back(_tiles[i][j]);
			}
		}

		bottomToTop = !bottomToTop;
	}
}

void Game::SetupPlayerSpawnArea(int offset)
{
	int rowLimit = offset;
	int colLimit = _maxRowCol - rowLimit - 1;
	int tileCounter = 0;

	for (int i = 1; i < rowLimit; i++)
	{
		for (int j = _maxRowCol - 2; j > colLimit; j--)
		{
			_tiles[i][j]->ChangeTile(Tile::Type::PlayerSpawn);
			_renderTiles.push_back(_tiles[i][j]);

			if (_player == nullptr)
			{
				SpawnPlayer(_tiles[i][j]);
			}

			tileCounter++;
		}
	}
	string message = "" + tileCounter;
	DEBUG_MSG("Player Spawn Tiles: ");
	DEBUG_MSG(tileCounter);
}
void Game::SetupNPC_SpawnArea(int offset, int spacing)
{
	int rowLimit = offset + (spacing * (_maxWalls - 1));
	int colLimit = _maxRowCol / 3;
	int tileCounter = 0;

	for (int i = _maxRowCol - 2; i > rowLimit; i--)
	{
		for (int j = _maxRowCol * 2 / 3; j > colLimit; j--)
		{
			_tiles[i][j]->ChangeTile(Tile::Type::NpcSpawn);
			_renderTiles.push_back(_tiles[i][j]);

			if (_npcs.size() < _maxNPC)
			{
				SpawnNPC(_tiles[i][j]);
			}

			tileCounter++;
		}
	}

	std::reverse(_npcs.begin(), _npcs.end());

	DEBUG_MSG("NPC Spawn Tiles: ");
	DEBUG_MSG(tileCounter);
}

void Game::SpawnPlayer(const Tile* tile)
{
	if (!tile->getOccupied())
	{
		SDL_Rect rectangle = CreateRectangle(tile);
		SDL_Color colour = CreateColour(tile, true);

		_player = new Player();
		_player->Initialize(rectangle, colour);
	}
}
void Game::SpawnNPC(const Tile* tile)
{
	if (!tile->getOccupied())
	{
		SDL_Rect rectangle = CreateRectangle(tile);
		SDL_Color colour = CreateColour(tile, false);

		_npcs.push_back(new NPC());
		_npcs[_npcs.size() - 1]->Initialize(rectangle, colour);
	}
}

SDL_Rect Game::CreateRectangle(const Tile* tile)
{
	SDL_Rect rectangle = SDL_Rect();

	int halfWidth = tile->getRectangle().w * 0.5f;
	int halfHeight = tile->getRectangle().h * 0.5f;

	rectangle.x = tile->getRectangle().x + halfWidth;
	rectangle.y = tile->getRectangle().y + halfHeight;
	rectangle.w = halfWidth;
	rectangle.h = halfHeight;

	return rectangle;
}
SDL_Color Game::CreateColour(const Tile* tile, bool isPlayer)
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

void Game::LoadContent()
{
	DEBUG_MSG("Loading Content - Nothing to load :(");
}

void Game::Render()
{
	SDL_RenderClear(_renderer);

	for (int i = 0; i < _renderTiles.size(); i++)
	{
		_renderTiles[i]->Render(_renderer, _camera);
	}

	for (int i = 0; i < _npcs.size(); i++)
	{
		_npcs[i]->Render(_renderer, _camera);
	}

	_player->Render(_renderer, _camera);

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

void Game::Update()
{
	//DEBUG_MSG("Updating....");
}

void Game::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					_running = false;
					break;

				case SDLK_UP:
					//DEBUG_MSG("Up Key Pressed");
					if (_camera.y > 0)
					{
						_camera.y -= 50;
					}
					break;
				case SDLK_DOWN:
					//DEBUG_MSG("Down Key Pressed");
					if (_camera.y + _camera.h < _worldBottomRightCorner)
					{
						_camera.y += 50;
					}
					break;
				case SDLK_LEFT:
					//DEBUG_MSG("Left Key Pressed");
					if (_camera.x > 0)
					{
						_camera.x -= 50;
					}
					break;
				case SDLK_RIGHT:
					//DEBUG_MSG("Right Key Pressed");
					if (_camera.x + _camera.w < _worldBottomRightCorner)
					{
						_camera.x += 50;
					}
					break;

				case SDLK_w:
					//DEBUG_MSG("Up Key Pressed");
					if (_camera.y > 0)
					{
						_camera.y -= 50;
					}
					break;
				case SDLK_s:
					//DEBUG_MSG("Down Key Pressed");
					if (_camera.y + _camera.h < _worldBottomRightCorner)
					{
						_camera.y += 50;
					}
					break;
				case SDLK_a:
					//DEBUG_MSG("Left Key Pressed");
					if (_camera.x > 0)
					{
						_camera.x -= 50;
					}
					break;
				case SDLK_d:
					//DEBUG_MSG("Right Key Pressed");
					if (_camera.x + _camera.w < _worldBottomRightCorner)
					{
						_camera.x += 50;
					}

				default:
					//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
					break;
				}
	}
}

bool Game::IsRunning()
{
	return _running;
}

void Game::UnloadContent()
{
	DEBUG_MSG("Unloading Content");
}

void Game::CleanUp()
{
	DEBUG_MSG("Cleaning Up");
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}
