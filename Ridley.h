#pragma once
#include "Boss.h"
#include "Port.h"
#include "BulletRidley.h"
class Ridley :public Boss
{
protected:
	bool isJump;
	int indexBullet;
public:
	BulletRidley *mListBullet[6];
	Ridley(){}
	~Ridley() 
	{
		for (int i = 0; i < 6; i++)
		{
			BulletRidley *bullet = mListBullet[i];
			delete bullet;
		}
	}
	Ridley(Sprite* sprite_boss, InfoSprite* info_boss, D3DXVECTOR2 pos);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void OnCollision(Object *obj, D3DXVECTOR2 distance);
	void OnCollision(Object *obj);
	void OnCollision();
	void ChangeAnimation(Keyboard* key);
	void New();
};