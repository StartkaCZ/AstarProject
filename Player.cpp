#include <Player.h>

Player::Player()
	: _wonderTimer(0)
{
	
}

Player::~Player()
{
	DEBUG_MSG("Destructing Player");
}

void Player::Initialize(SDL_Rect rectangle, SDL_Color colour)
{
	_rectangle = rectangle;
	_colour = colour;

	_initialX = _rectangle.x;
	_initialY = _rectangle.y;

	_goalX = _initialX;
	_goalY = _initialY;
}

void Player::Update()
{
	//DEBUG_MSG("Player Updating");
}
void Player::Update(vector<vector<Tile*>>& tiles, int tileSize, int dt)
{
	if (_wonderTimer > WONDER_TIMER)
	{
		_rectangle.x = _goalX;
		_rectangle.y = _goalY;

		Wonder(tiles, tileSize);
		_wonderTimer = 0;
	}
	else
	{
		_wonderTimer += dt / 1000.f;
		Interpolate();
	}
}
void Player::Interpolate()
{
	float scale = _wonderTimer / WONDER_TIMER;

	_rectangle.x = _initialX + (_goalX - _initialX) * scale;
	_rectangle.y = _initialY + (_goalY - _initialY) * scale;
}

void Player::Wonder(vector<vector<Tile*>>& tiles, int tileSize)
{
	int i = _rectangle.x / tileSize;
	int j = _rectangle.y / tileSize;

	int randomDirection = rand() % 4;

	bool canMoveToTile = false;

	while (!canMoveToTile)
	{
		Direction randomDirection = static_cast<Direction>(rand() % 4);

		if (randomDirection == Direction::Right)
		{
			Move(tiles[i + 1][j], canMoveToTile, randomDirection);
		}
		else if (randomDirection == Direction::Left)
		{
			Move(tiles[i - 1][j], canMoveToTile, randomDirection);
		}
		else if (randomDirection == Direction::Up)
		{
			Move(tiles[i][j + 1], canMoveToTile, randomDirection);
		}
		else
		{
			Move(tiles[i][j - 1], canMoveToTile, randomDirection);
		}
	}
	tiles[i][j]->SetOccupied(false);

}
void Player::Move(Tile* tile, bool& canMoveToTile, Direction randomDirection)
{
	if (!tile->getOccupied())
	{
		_lastDirectionMoved = randomDirection;
		canMoveToTile = true;

		int halfWidth = tile->getRectangle().w * 0.5f;
		int halfHeight = tile->getRectangle().h * 0.5f;
		
		_goalX = tile->getRectangle().x + (halfWidth * 0.5f);
		_goalY = tile->getRectangle().y + (halfHeight * 0.5f);

		_initialX = _rectangle.x;
		_initialY = _rectangle.y;

		tile->SetOccupied(true);
	}
}


void Player::CleanUp()
{
	DEBUG_MSG("Cleaning Up Player");
}
