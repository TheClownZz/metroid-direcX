#include "Tile.h"

Tile::Tile()
{

}
Tile::Tile(Graphic* graphic, int tileperrow, float widthtile, float heighttile)
{
	sprite = new Sprite(graphic, TileSetPNG);
	WidthTile = widthtile;
	HeightTile = heighttile;
	TilePerRow = tileperrow;
}


Tile::~Tile()
{
}
int Tile::GetId()
{
	return this->Id;
}
void Tile::SetId(int id, D3DXVECTOR2 position)
{
	this->Id = id;
	this->sprite->SetPosition(position);
}
void Tile::SetTileRect()
{
	RECT r;
	r.left = (Id % TilePerRow)*WidthTile;
	r.top = (Id / TilePerRow)*HeightTile;
	r.right = r.left + WidthTile;
	r.bottom = r.top + HeightTile;
	sprite->SetRect(r);
}
void Tile::Render(Viewport* view)
{
	sprite->Render(view);
}
