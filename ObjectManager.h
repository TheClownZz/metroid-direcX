#pragma once
#include "Graphic.h"
#include "Keyboard.h"
#include "Sound.h"
#include "SamusAran.h"
#include "Map.h"
#include "Viewport.h"
#include "Collision.h"
#include "Ridley.h"
#include "Kraid.h"
#include "GUI.h"
#include <vector>
#include "GameDefine.h"

using namespace Define;

class ObjectManager
{
protected:
	Viewport* viewport;
	SamusAran* Samus;
	Map* map;
	GUI* gui;
	QuadTree *mQuadTree;

	//Sound
	Sound *sound;
	GSound* soundEnter;
	GSound* soundBrinstar;
	GSound* soundpowerup;
	GSound* soundSamusColli;
	GSound* soundRipperColli;
	GSound* soundEnemyColli;
		

	vector <Object*> listObjectCollison;
	vector <Object*> list;
	vector <Object*> listEnemy;
	vector <Object*> listWall;
	vector <RECT> listScene;
	D3DXVECTOR2 posView;
	Ridley *mRidley;
	Kraid *mKraid;
	Sprite* sprite_Samus;
	Sprite* sprite_bullet;
	Sprite* sprite_enemy;
	Sprite* sprite_port;
	Sprite* sprite_item;
	Sprite* sprite_boss;

	InfoSprite* info_Samus;
	InfoSprite* info_bullet;
	InfoSprite* info_enemy;
	InfoSprite* info_port;
	InfoSprite* info_item;
	InfoSprite* info_boss;

	float timepause;
	bool pause, allowpause;

public:
	ObjectManager();
	~ObjectManager();

	void ReadQuadTree(TiXmlElement *root, QuadTree *node, QuadTree *father, int indexNode);
	//Load Data Game
	void InitDT(Graphic* graphic, Sound *sound);

	//Update Game sau khoảng thời gian gameTime
	void Update(float gameTime, Keyboard* key);

	//Vẽ Object lên màn hình
	void Render();

	void Start();
	int End();
};
