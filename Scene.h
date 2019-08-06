#pragma once
#include "Graphic.h"
#include "Keyboard.h"
#include "ObjectManager.h"
#include "Sound.h"
#include "GameDefine.h"

using namespace Define;

class Scene
{
protected:
	Sprite* sprite;
	InfoSprite* info_scene;
	Animation* MenuAnimation;
	D3DXVECTOR2 positionMenu;
	float timedelay;

	//Sound
	Sound *sound;
	 
	GSound* soundTitle;
	GSound* soundEnd;

	ObjectManager* ObjManager;
public:

	enum Scenestage
	{
		TileScreen,
		Menu,
		Play,
		Gameover,
		EndGame
	} SceneStage;
	Scene();
	~Scene();

	//Load Data Game
	void InitDT(Graphic* graphic);

	//Update Game sau khoảng thời gian gameTime
	void Update(float gameTime, Keyboard* key);

	//Vẽ Object lên màn hình
	void Render();
};

