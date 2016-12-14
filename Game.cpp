#include <Game.h>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "LTimer.h"
#include "ConstHolder.h"

using namespace std;

int Game::_threadsFinished = 0;
SDL_bool Game::_canWork = SDL_TRUE;
SDL_mutex* Game::_mutex = SDL_CreateMutex();
SDL_semaphore* Game::_semaphore = SDL_CreateSemaphore(1);

Game::Game() 
	: _running(false)
	, _player(nullptr)
	, _camera(nullptr)
	, _data(nullptr)
	, _threads(vector<SDL_Thread*>())
	, _npcs(vector<NPC*>())
	, _threadJobDoneLog(new map<string, int>())
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

		_threadIt = true;
		_hasThreaded = false;
		
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
	_data = new Data();
	_data->grid = new Grid();

	_level = new Level(_currentLevel);
	_level->Initialize(_player, _npcs, _data->grid->getTiles(), worldBottomRightCorner, 600, 600);

	_npcs.shrink_to_fit();

	_data->tileSize =_level->getTileSize();

	_threadsFinished = 0;
	_canWork = SDL_TRUE;
	
	for (int i = 0; i < MAX_THREADS_TO_RUN; i++)
	{
		std::ostringstream oss;
		oss << i;

		string threadName = "Worker" + oss.str();

		std::pair<string, int> jobDoneLog = std::pair<string, int>(threadName, 0);
		_threadJobDoneLog->insert(jobDoneLog);

		Logger* logger = new Logger(_threadJobDoneLog);
		logger->threadName = threadName;
		logger->data = _data;


		SDL_Thread* worker = SDL_CreateThread(Worker, threadName.c_str(), logger);
		_threads.push_back(worker);
	}


	for (int i = 0; i < _npcs.size(); i++)
	{
		_data->jobs.push(getJob(i));

		/*_npcs[i]->SetPath(
			_Grid->CalculateAstar(_player->getRectangle().x,
				_player->getRectangle().y,
				_npcs[i]->getRectangle().x,
				_npcs[i]->getRectangle().y,
				_level->getTileSize(),
				"Test"));*/
	}
}

void Game::Render()
{
	SDL_RenderClear(_renderer);
	
	_data->grid->Render(_renderer, _camera->getRectangle(), _level->getTileSize());
	

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


	//_player->Update(_Grid->getTiles(), _level->getTileSize(), deltaTime);

	for (int i = 0; i < _npcs.size(); i++)
	{
		_npcs[i]->Update(_level->getTileSize(), deltaTime);
	}

	for (int i = 0; i < _npcs.size(); i++)
	{
	//	_jobs.push(_npcs[i]);
	}


	//save the curent time for next frame
	_lastTime = currentTime;
}

Game::Job* Game::getJob(int i)
{
	Job* job = new Job();

	job->_npc = _npcs[i];
	job->playerX = _player->getRectangle().x;
	job->playerY = _player->getRectangle().y;

	return job;
}

int Game::Worker(void* ptr)
{
	Logger* logger = static_cast<Logger*>(ptr);

	while (_canWork)
	{
		if (logger->data->jobs.size() > 0 && _canWork)
		{
			Job* job = nullptr;


			SDL_SemWait(_semaphore);
			if (logger->data->jobs.size() > 0)
			{
				logger->threadJobDoneCounter->at(logger->threadName)++;

				job = logger->data->jobs.front();
				logger->data->jobs.pop();
			}
			SDL_SemPost(_semaphore);


			if (job != nullptr)
			{
				job->_npc->SetPath(
					logger->data->grid->CalculateAstar(job->playerX, 
													   job->playerY, 
													   job->_npc->getRectangle().x, 
													   job->_npc->getRectangle().y, 
													   logger->data->tileSize,
													   logger->threadName) );
				
				delete job;
			}
		}
	}

	while (SDL_LockMutex(_mutex) != 0)
	{

	}

	_threadsFinished++;

	SDL_UnlockMutex(_mutex);
	DEBUG_MSG("FINISHED");

	return 0;
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
	PrintThreadJobsDone();

	SDL_UnlockMutex(_mutex);
	_canWork = SDL_FALSE;

	for (int i = 0; i < _threads.size(); i++)
	{
		SDL_DetachThread(_threads[i]);
	}

	_threads.clear();

	while (_threadsFinished < MAX_THREADS_TO_RUN)
	{
		
	}
	DEBUG_MSG("ALL THREADS FINISHED");

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

	_data->grid->Destroy();
	delete _data->grid;

	delete _level;
	_level = nullptr;
	delete _player;
	_player = nullptr;
	delete _data;
	_data = nullptr;

	while (_data->jobs.size() > 0)
	{
		_data->jobs.pop();
	}

	for (int i = 0; i < _npcs.size(); i++)
	{
		delete _npcs[i];
	}
	_npcs.clear();

	int worldBottomRightCorner = 0;
	CreateWorld(worldBottomRightCorner);
	_camera->ReInitialize(worldBottomRightCorner);
}

void Game::CleanUp()
{
	PrintThreadJobsDone();

	DEBUG_MSG("Cleaning Up");
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void Game::PrintThreadJobsDone()
{
	for (int i = 0; i < _threadJobDoneLog->size(); i++)
	{
		std::ostringstream oss;
		oss << i;

		string threadName = "Worker" + oss.str();

		DEBUG_MSG(threadName);
		DEBUG_MSG("Worked");
		DEBUG_MSG(_threadJobDoneLog->at(threadName));
	}

	for (int i = 0; i < _threadJobDoneLog->size(); i++)
	{
		std::ostringstream oss;
		oss << i;

		string threadName = "Worker" + oss.str();

		_threadJobDoneLog->at(threadName) = 0;
	}
}
