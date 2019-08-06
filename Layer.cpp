#include"Layer.h"

Layer::Layer()
{

}
Layer::Layer(Graphic g, char * path)
{
	//this->tileset = new TileSet(graphic, info->tileColumns, info->tileWidth, info->tileHeight);
	position = D3DXVECTOR2(0, 0);
}
Layer::~Layer()
{
	delete this->tileset;
}
float Layer::GetLayerHeight()
{
	return this->layerHeight;
}
float Layer::GetLayerWidth()
{
	return this->layerWidth;
}
string Layer::GetName()
{
	return name;
}
void Layer::Render(Viewport *View)
{
/*	for (int h = info->height - 1; h >= 0; h--)
		for (int w = 0; w < info->width; w++)
		{

			if (info->data[h][w] != 0)
			{
				D3DXVECTOR2 position(w * info->tileWidth, (info->height - h) * info->tileHeight);
				tileset->SetId(info->data[h][w], position);
				tileset->SetTileRect();
				tileset->Render(View);
			}

		}*/
}