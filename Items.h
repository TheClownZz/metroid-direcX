#pragma once
#include "SamusAran.h"
#include "Animation.h"
#include "GameDefine.h"
#include "Collision.h"
using namespace Define;

class Items : public Object
{
protected:
	GSound* sounditemHP;
	GSound* sounditemrocket;

	Animation* AnimationItem;
	SamusAran *Samus;
public:
	enum Itemtype
	{
		HpItem, //Ăn tăng máu
		RocketItem,// Ăn tăng đạn rocket khi cờ rocket hoạt động
		Marimaru,// Ăn cờ lăn bật
		Rocket,// Ăn thì chuyển được rocket
		Icebean,//Súng băng
		Boom,//Ăn thì lăn thả boom đc
		Longbean,//Đạn max tầm
		Tank,//Max HP và thêm 1 bình chứa máu 100HP
		Varia,//Chuyển sang trang phục varia không biết công dụng
		None
	};
	Itemtype ItemType;

	Items();
	~Items();
	Items(SamusAran *Samus, Sprite* _sprite, InfoSprite* info_item, Sound* sound);

	void ChangeAnimation(Keyboard* key);
	void  OnCollision();
	void NewItems(D3DXVECTOR2 pos, Itemtype type);
	static Itemtype GetType(string name);
	void Update(float gameTime, Keyboard *key);
	void Render(Viewport *view);
};

