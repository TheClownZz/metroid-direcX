#pragma once
#include "Sprite.h"
#include "Viewport.h"
#include "Graphic.h"

class Tile
{
protected:
	Sprite* sprite;
	float WidthTile, HeightTile;
	int Id, TilePerRow;
public:
	Tile();
	Tile(Graphic* graphic, int tileperrow, float widthtile, float heighttile);
	~Tile();
	
	int GetId();
	void SetId(int id, D3DXVECTOR2 position);
	void SetTileRect();

	void Render(Viewport* view);
};