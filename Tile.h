#ifndef TILE_H
#define TILE_H

#include "GameObject.h"

#include <map>

class Tile : public GameObject
{
public:
	enum class Type
	{
		Empty,
		Wall,
		PlayerSpawn,
		NpcSpawn,


		ClosedList,
		Path,
	};

	struct NodeData
	{
		NodeData()
			: indexX(-1)
			, indexY(-1)
			, parentIndexX(-1)
			, parentIndexY(-1)
			, g(999999)
			, h(-1)
			, f(-1)
			, open(false)
			, closed(false)
		{		
		
		}

		void SetIndex(int x, int y)
		{
			indexX = x;
			indexY = y;
		}

		void SetParentIndex(int x, int y)
		{
			parentIndexX = x;
			parentIndexY = y;
		}

		void Restart()
		{
			indexX = -1;
			indexY = -1;
			parentIndexX = -1;
			parentIndexY = -1;
			g = 999999;
			h = -1;
			f = -1;
			open = false;
			closed = false;
		}

		int indexX;
		int indexY;

		int parentIndexX;
		int parentIndexY;

		float g;
		float h;
		float f;

		bool open;
		bool closed;
	};


							Tile();
							~Tile();
	void					Initialize(SDL_Rect rectangle, Type type);
	void					ChangeTile(Type type);
	void					Render(SDL_Renderer*& sdl_renderer, const SDL_Rect& camera) override;
	void					Update() override;
	void					CleanUp() override;
	
	void					SetOccupied(bool isOccupied);

	Type					getType() const;
	bool					getOccupied() const;
	map<string, NodeData>&	getNodeData();

private:
	map<string, NodeData>	_nodeData;
	Type					_type;
	bool					_isOccupied;
};
#endif