#include <Game.h>
#include <iostream>

#include "LTimer.h"

using namespace std;

Grid* Game::_Grid = nullptr;
queue<NPC*>Game::_jobs = queue<NPC*>();
SDL_semaphore* Game::_semaphore = SDL_CreateSemaphore(1);

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
		_currentLevel = 1;
		DEBUG_MSG("LEVEL: ");
		DEBUG_MSG(_currentLevel);

		SDL_Rect cameraRectangle = SDL_Rect();
		int worldBottomRightCorner;

		cameraRectangle.x = 0;
		cameraRectangle.y = 0;
		cameraRectangle.w = width;
		cameraRectangle.h = height;

		_Grid = new Grid();
		_npcs = vector<NPC*>();

		_level = new Level(_currentLevel);
		_level->Initialize(_player, _npcs, _Grid->getTiles(), worldBottomRightCorner, width, height);

		_camera = new Camera();
		_camera->Initialize(cameraRectangle, worldBottomRightCorner);

		_npcs.shrink_to_fit();

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

void Game::Render()
{
	SDL_RenderClear(_renderer);
	
	_Grid->Render(_renderer, _camera->getRectangle(), _level->getTileSize());
	

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


	_player->Update(_Grid->getTiles(), _level->getTileSize(), deltaTime);

	for (int i = 0; i < _npcs.size(); i++)
	{
		_npcs[i]->Update(_level->getTileSize(), deltaTime);
	}

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
		

		SDL_SemWait(_semaphore);
		if (_jobs.size() > 0)
		{
			npc = _jobs.front();
			_jobs.pop();
		}
		SDL_SemPost(_semaphore);


		if (npc != nullptr)
		{
			npc->SetPath(_Grid->CalculateAstar());
		}
	}
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

void Game::CleanUp()
{
	DEBUG_MSG("Cleaning Up");
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}
