#include <Game.h>
#include <iostream>

#include "LTimer.h"

using namespace std;

queue<NPC*>Game::_jobs = queue<NPC*>();
int Game::_mutex = 1;
bool Game::_lock = false;

Game::Game() 
	: _running(false)
	, _player(nullptr)
	, _camera(nullptr)
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
		_currentLevel = 0;
		DEBUG_MSG("LEVEL: ");
		DEBUG_MSG(_currentLevel);

		SDL_Rect cameraRectangle = SDL_Rect();
		int worldBottomRightCorner;

		cameraRectangle.x = 0;
		cameraRectangle.y = 0;
		cameraRectangle.w = width;
		cameraRectangle.h = height;

		_npcs = vector<NPC*>();
		_tiles = vector<vector<Tile*>>();

		_level = new Level(_currentLevel);
		_level->Initialize(_player, _npcs, _tiles, worldBottomRightCorner, width, height);

		_camera = new Camera();
		_camera->Initialize(cameraRectangle, worldBottomRightCorner);

		_npcs.shrink_to_fit();
		_tiles.shrink_to_fit();

		for (int i = 0; i < 8; i++)
		{
			SDL_Thread* worker = SDL_CreateThread(Worker, "Worker" + i, (void*)NULL);
		}
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

void Game::LoadContent()
{
	DEBUG_MSG("Loading Content - Nothing to load :(");
}

void Game::Render()
{
	SDL_RenderClear(_renderer);

	int cameraLeftPosition = _camera->getRectangle().x / _level->getTileSize();
	int cameraBottomPosition = _camera->getRectangle().y / _level->getTileSize();
	int cameraRightPosition = (_camera->getRectangle().x + _camera->getRectangle().w) / _level->getTileSize();
	int cameraTopPosition = (_camera->getRectangle().y + _camera->getRectangle().h) / _level->getTileSize();

	for (int i = cameraLeftPosition; i < cameraRightPosition; i++)
	{
		for (int j = cameraBottomPosition; j < cameraTopPosition; j++)
		{
			_tiles[i][j]->Render(_renderer, _camera->getRectangle());
		}
	}

	for (int i = 0; i < _npcs.size(); i++)
	{
		_npcs[i]->Render(_renderer, _camera->getRectangle());
	}

	_player->Render(_renderer, _camera->getRectangle());

	SDL_SetRenderDrawColor(_renderer, 55, 55, 55, 255);
	SDL_RenderPresent(_renderer);
}

void Game::Update()
{
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	unsigned int deltaTime = currentTime - _lastTime;//time since last update


	_player->Update(_tiles, _level->getTileSize(), deltaTime);


	for (int i = 0; i < _npcs.size(); i++)
	{
		_jobs.push(_npcs[i]);
	}


	//save the curent time for next frame
	_lastTime = currentTime;
}

int Game::Worker(void* ptr)
{
	while (true)
	{
		
		NPC* npc = nullptr;

		P();

		while (_lock)
		{
		}
		
		_lock = true;
		if (_jobs.size() > 0)
		{
			//DEBUG_MSG("Inside:");
			npc = _jobs.front();
			_jobs.pop();
			//DEBUG_MSG("Done:");
		}
		_lock = false;

		V();

		if (npc != nullptr)
		{
			DEBUG_MSG("Calculating A*...");
			npc->CalculateAstar();
		}
	}
}

void Game::P()
{
	DEBUG_MSG("Mutex: ");
	DEBUG_MSG(_mutex);

	while (_mutex == 0)
	{
		//await
		DEBUG_MSG("Waiting...");
	}

	_mutex--;
}
void Game::V()
{
	_mutex++;
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
					_camera->Move(Direction::Up);
					break;
				case SDLK_DOWN:
					//DEBUG_MSG("Down Key Pressed");
					_camera->Move(Direction::Down);
					break;
				case SDLK_LEFT:
					//DEBUG_MSG("Left Key Pressed");
					_camera->Move(Direction::Left);
					break;
				case SDLK_RIGHT:
					//DEBUG_MSG("Right Key Pressed");
					_camera->Move(Direction::Right);
					break;

				case SDLK_w:
					//DEBUG_MSG("Up Key Pressed");
					_camera->Move(Direction::Up);
					break;
				case SDLK_s:
					//DEBUG_MSG("Down Key Pressed");
					_camera->Move(Direction::Down);
					break;
				case SDLK_a:
					//DEBUG_MSG("Left Key Pressed");
					_camera->Move(Direction::Left);
					break;
				case SDLK_d:
					//DEBUG_MSG("Right Key Pressed");
					_camera->Move(Direction::Right);

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
