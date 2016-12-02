#include <NPC.h>

NPC::NPC()
	: _closedList(list<Node>())
	, _openList(priority_queue<Node>())
{ 
	//DEBUG_MSG("Constructing NPC");
}

NPC::~NPC()
{
	DEBUG_MSG("Destructing NPC");
}
/*
void NPC::Initialize(SDL_Rect rectangle, SDL_Color colour)
{
DEBUG_MSG("NPC Initializing");

}

void NPC::Render()
{
DEBUG_MSG("NPC Drawing");
}
*/
void NPC::Update()
{
	DEBUG_MSG("NPC Updating");
}

void NPC::CleanUp()
{
	DEBUG_MSG("Cleaning Up NPC");
}

void NPC::CalculateAstar()
{
	//DEBUG_MSG("Calculating A*...");
	_closedList.clear();
	//_openList.push;

	//DEBUG_MSG("... A* Finished");
}