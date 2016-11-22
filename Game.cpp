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
		SetupLevel(width, height);
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
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
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
void Game::SetupLevel(int width, int height)
{
	_level = 2;

	_camera.x = 0;
	_camera.y = 0;
	_camera.w = width;
	_camera.h = height;

	_worldScale = 1.0f;
	_maxRowCol = 30;
	_maxNPC = 5;
	int maxWalls = 3;
	int isToTouchWalls = 1;

	if (_level == 1)
	{
		_worldScale = 1.0f;
		_maxRowCol = 100;
		_maxNPC = 50;
		maxWalls = 6;
		isToTouchWalls = 2;
	}
	else if (_level == 2)
	{
		_worldScale = 5.0f;
		_maxRowCol = 1000;
		_maxNPC = 500;
		maxWalls = 18;
		isToTouchWalls = 4;
	}

	//tile size = ((width + height) * 0.5 * 4) / maxRowCol
	// scale the world 4 times
	int tileSize = ((width + height) * 0.5f * _worldScale) / _maxRowCol;
	_worldBottomRightCorner = tileSize * _maxRowCol;

	_npcs = vector<NPC*>(_maxNPC);
	_tiles = vector<vector<Tile*>>(_maxRowCol);

	SDL_Rect setupRectangle = SDL_Rect();
	SDL_Color setupColour = SDL_Color();

	int x = 0;
	int y = 0;

	//tiles available = row/col max tiles - (edge + edge) - (spacing for player/NPC area)
	int spawnAreaTiles = _maxRowCol * 0.2f;
	int tilesAvailble = _maxRowCol - 2 - spawnAreaTiles - maxWalls;
	int spacing = tilesAvailble / (maxWalls-1);
	int offset = 2 + spawnAreaTiles * 0.5f;

	for (int i = 0; i < _maxRowCol; i++)
	{
		for (int j = 0; j < _maxRowCol; j++)
		{
			bool isPassible = true;

			setupRectangle.x = x;
			setupRectangle.y = y;
			setupRectangle.h = tileSize;
			setupRectangle.w = tileSize;

			if (i == 0 || j == 0 || i == _maxRowCol-1 || j == _maxRowCol-1)
			{
				isPassible = false;
			}			

			_tiles[i].push_back(new Tile());
			_tiles[i][j]->Initialize(setupRectangle, isPassible);
			y += tileSize;
		}

		y = 0;
		x += tileSize;
	}


	int tilesToWalls = (_maxRowCol-2) * 0.9f;
	bool bottomToTop = rand() % 2 == 0;

	for (int k = 0; k < maxWalls; k++)
	{
		int i = offset + (spacing * k);

		if (bottomToTop)
		{
			for (int j = 1; j < tilesToWalls; j++)
			{
				_tiles[i][j]->ChangeTile(false);
			}
		}
		else
		{
			int amount = _maxRowCol - tilesToWalls - 2;

			for (int j = _maxRowCol - 1; j > amount; j--)
			{
				_tiles[i][j]->ChangeTile(false);
			}
		}

		bottomToTop = !bottomToTop;
	}
}


void Game::LoadContent()
{
	DEBUG_MSG("Loading Content - Nothing to load :(");
}

void Game::Render()
{
	SDL_RenderClear(_renderer);
	
	for (int i = 0; i < _maxRowCol; i++)
	{
		for (int j = 0; j < _maxRowCol; j++)
		{
			_tiles[i][j]->Render(_renderer, _camera);
		}
	}
	
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
		switch(event.type)
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					_running = false;
					break;

				case SDLK_UP:
					//DEBUG_MSG("Up Key Pressed");
					if (_camera.y > 0)
					{
						_camera.y-=50;
					}
					break;
				case SDLK_DOWN:
					//DEBUG_MSG("Down Key Pressed");
					if (_camera.y + _camera.h < _worldBottomRightCorner)
					{
						_camera.y+= 50;
					}
					break;
				case SDLK_LEFT:
					//DEBUG_MSG("Left Key Pressed");
					if (_camera.x > 0)
					{
						_camera.x-= 50;
					}
					break;
				case SDLK_RIGHT:
					//DEBUG_MSG("Right Key Pressed");
					if (_camera.x + _camera.w < _worldBottomRightCorner)
					{
						_camera.x+= 50;
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
					SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
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
