#include <Game.h>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "LTimer.h"
#include "ConstHolder.h"
#include "FPS_Counter.h"

using namespace std;

Game::Game() 
	: _running(false)
	, _player(nullptr)
	, _camera(nullptr)
	, _threadIt(true)
	, _fpsTimer(0)
	, _npcs(vector<NPC*>())
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

		_hasThreaded = _threadIt;
		
		cameraRectangle.x = 0;
		cameraRectangle.y = 0;
		cameraRectangle.w = width;
		cameraRectangle.h = height;

		CreateWorld(worldBottomRightCorner);

		_camera = new Camera();
		_camera->Initialize(cameraRectangle, worldBottomRightCorner);
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
void Game::CreateWorld(int& worldBottomRightCorner)
{
	_grid = new Grid();

	_level = new Level(_currentLevel);
	_level->Initialize(_player, _npcs, _grid->getTiles(), worldBottomRightCorner, 600, 600);

	_npcs.shrink_to_fit();

	if (_hasThreaded)
	{
		ThreadPool::Instance()->Setup(_grid, _level->getTileSize());

		/*for (int i = 0; i < _npcs.size(); i++)
		{
			if (_npcs[i]->IsPathComplete())
			{
				ThreadPool::Instance()->setJob(_npcs[i], _player->getRectangle().x, _player->getRectangle().y);
			}
		}*/
	}

	PrintFPS();
}

void Game::Render()
{
	SDL_RenderClear(_renderer);
	
	_grid->Render(_renderer, _camera->getRectangle(), _level->getTileSize());
	

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


	int timesPlayerReached = 0;

	for (int i = 0; i < _npcs.size(); i++)
	{
		_npcs[i]->Update(_level->getTileSize(), deltaTime);

		if (_npcs[i]->IsPathComplete())
		{
			if (_hasThreaded)
			{
				ThreadPool::Instance()->setJob(_npcs[i], _player->getRectangle().x, _player->getRectangle().y);
			}
			else
			{
				_npcs[i]->SetPath(_grid->CalculateAstar(_player->getRectangle().x,
														_player->getRectangle().y,
														_npcs[i]->GetGoalX(),
														_npcs[i]->GetGoalY(),
														_level->getTileSize(),
														"-1"));
			}
		}
		else if (_npcs[i]->HasReachedPlayer())
		{
			timesPlayerReached++;
		}
	}

	int distance = abs(_npcs[0]->getRectangle().x - _player->getRectangle().x) + abs(_npcs[0]->getRectangle().y - _player->getRectangle().y);
	distance /= _level->getTileSize();

	if (timesPlayerReached == 0 && distance > 6)
	{
		_player->Update(_grid->getTiles(), _level->getTileSize(), deltaTime);
	}

	if (timesPlayerReached == _npcs.size())
	{
		NewLevel(-1);
	}

	PrintFPSTimer(deltaTime);

	//save the curent time for next frame
	_lastTime = currentTime;
}
void Game::PrintFPSTimer(int dt)
{
	if (_fpsTimer > 1)
	{
		PrintFPS();
		_fpsTimer = 0;
	}
	else
	{
		_fpsTimer += dt / 1000.0f;
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
					break;

				case SDLK_1:
					NewLevel(0);
					break;
				case SDLK_2:
					NewLevel(1);
					break;
				case SDLK_3:
					NewLevel(2);
					break;
				case SDLK_0:
					NewLevel(-1);
					break;

				case SDLK_q:
					_threadIt = false;
					DEBUG_MSG("DO NOT THREAD IT NEXT LEVEL");
					break;
				case SDLK_e:
					_threadIt = true;
					DEBUG_MSG("DO THREAD IT NEXT LEVEL");
					break;

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


void Game::NewLevel(int level)
{
	FreeMemory();

	if (level == -1)
	{
		_currentLevel++;

		if (_currentLevel > 2)
		{
			_currentLevel = 0;
		}
	}
	else
	{
		_currentLevel = level;
	}

	DEBUG_MSG("THREAD IT?");
	DEBUG_MSG(_threadIt);
	_hasThreaded = _threadIt;

	int worldBottomRightCorner = 0;
	CreateWorld(worldBottomRightCorner);
	_camera->ReInitialize(worldBottomRightCorner);
}

void Game::CleanUp()
{
	FreeMemory();

	DEBUG_MSG("Cleaning Up");
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void Game::PrintFPS()
{
	DEBUG_MSG("FPS: Average, Highest, Lowest");
	DEBUG_MSG(FPS_Counter::Instance()->getFramesPerSecond());
	DEBUG_MSG(FPS_Counter::Instance()->getHighestFrame());
	DEBUG_MSG(FPS_Counter::Instance()->getLowestFrame());
}

void Game::FreeMemory()
{
	PrintFPS();

	if (_hasThreaded)
	{
		ThreadPool::Instance()->Destroy();
	}

	_grid->Destroy();
	delete _grid;
	_grid = nullptr;

	delete _level;
	_level = nullptr;
	delete _player;
	_player = nullptr;


	for (int i = 0; i < _npcs.size(); i++)
	{
		delete _npcs[i];
	}
	_npcs.clear();
}
