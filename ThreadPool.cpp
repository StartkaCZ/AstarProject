#include "ThreadPool.h"

#include "ConstHolder.h"

#include <sstream>
#include <thread>


ThreadPool *ThreadPool::_instance = nullptr;

int ThreadPool::_threadsFinished = 0;
SDL_bool ThreadPool::_canWork = SDL_TRUE;
SDL_mutex* ThreadPool::_jobMutex = SDL_CreateMutex();
SDL_mutex* ThreadPool::_countMutex = SDL_CreateMutex();
SDL_semaphore* ThreadPool::_semaphore = nullptr;


/**Singleton instance*/
ThreadPool* ThreadPool::Instance()
{
	if (_instance == nullptr)
	{//if our instance hasn't been initialized
	 //initialize it
		_instance = new ThreadPool();
	}
	//return the instance.
	return _instance;
}


ThreadPool::ThreadPool()
	: _threads(vector<SDL_Thread*>())
	, _threadJobDoneLog(new map<string, int>())
{
}

ThreadPool::~ThreadPool()
{
	
}

void ThreadPool::Setup(Grid*& grid, int tileSize)
{
	_threadsFinished = 0;
	_canWork = SDL_TRUE;

	_grid = grid;
	_tileSize = tileSize;

	_semaphore = SDL_CreateSemaphore(0);

	SDL_UnlockMutex(_jobMutex);

	int maxThreads = thread::hardware_concurrency() - 1;
	for (int i = 0; i < maxThreads; i++)
	{
		std::ostringstream oss;
		oss << i;

		string threadName = oss.str();

		std::pair<string, int> jobDoneLog = std::pair<string, int>(threadName, 0);
		_threadJobDoneLog->insert(jobDoneLog);

		SDL_Thread* worker = SDL_CreateThread(Worker, threadName.c_str(), (void*)threadName.c_str());
		_threads.push_back(worker);
	}
}


void ThreadPool::setJob(NPC*& npc, int playerX, int playerY)
{
	Job* job = new Job(npc, playerX, playerY);

	_jobs.push(job);

	SDL_SemPost(_semaphore);
}

int ThreadPool::Worker(void* ptr)
{
	string name((char*)ptr);

	while (_canWork)
	{
		SDL_SemWait(_semaphore);

		Job* job = nullptr;

		while (SDL_LockMutex(_jobMutex) != 0)
		{

		}

		ThreadPool::Instance()->RequestJob(job);

		SDL_UnlockMutex(_jobMutex);


		if (job != nullptr)
		{
			job->_npc->SetPath(
				ThreadPool::Instance()->getGrid()->CalculateAstar(job->playerX,
																  job->playerY,
																  job->_npc->GetGoalX(),
																  job->_npc->GetGoalY(),
																  ThreadPool::Instance()->getTileSize(),
																  name));

			delete job;
			job = nullptr;
		}
	}

	while (SDL_LockMutex(_countMutex) != 0)
	{

	}

	_threadsFinished++;

	SDL_UnlockMutex(_countMutex);
	DEBUG_MSG("FINISHED");

	return 0;
}

void ThreadPool::RequestJob(Job*& job)
{
	if (_canWork)
	{
		job = _jobs.front();
		_jobs.pop();
	}
}

Grid*& ThreadPool::getGrid()
{
	return _grid;
}

int ThreadPool::getTileSize()
{
	return _tileSize;
}

void ThreadPool::Destroy()
{
	PrintThreadJobsDone();

	SDL_UnlockMutex(_countMutex);
	_canWork = SDL_FALSE;


	int postsToDo = _threadJobDoneLog->size() - _jobs.size();
	if (postsToDo > 0)
	{
		for (int i = 0; i < postsToDo; i++)
		{
			SDL_SemPost(_semaphore);
		}
	}


	for (int i = 0; i < _threads.size(); i++)
	{
		SDL_DetachThread(_threads[i]);
	}

	_threads.clear();

	while (_threadsFinished < _threadJobDoneLog->size())
	{

	}
	DEBUG_MSG("ALL THREADS FINISHED");

	SDL_DestroySemaphore(_semaphore);

	while (_jobs.size() > 0)
	{
		_jobs.pop();
	}

	delete _instance;
	_instance = nullptr;
}

void ThreadPool::PrintThreadJobsDone()
{
	for (int i = 0; i < _threadJobDoneLog->size(); i++)
	{
		std::ostringstream oss;
		oss << i;

		string threadName = oss.str();

		DEBUG_MSG("Thread");
		DEBUG_MSG(threadName);
		DEBUG_MSG("Worked");
		DEBUG_MSG(_threadJobDoneLog->at(threadName));

		_threadJobDoneLog->at(threadName) = 0;
	}
}