#pragma once
#include "Viewport.h"
#include "TileSet.h"
#include "GameDefine.h"
class Layer
{
protected:
	TileSet* tileset;
	D3DXVECTOR2 position;
	string name;
	float layerWidth, layerHeight;
	int data[500][300];
public:
	Layer();
	Layer(Graphic g, char * path);
	~Layer();
	string GetName();
	float GetLayerWidth();
	float GetLayerHeight();
	void Render(Viewport *View);
};
