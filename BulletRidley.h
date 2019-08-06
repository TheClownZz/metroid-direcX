#pragma once
#include "Object.h"
#include "Animation.h"
#include "GameDefine.h"
#include"Collision.h"

using namespace Define;

class BulletRidley : public Object
{
protected:
	Animation* mAnimation;
	D3DXVECTOR2 posStart;

public:

	BulletRidley() {};
	~BulletRidley()
	{
		delete mAnimation;
	}
	BulletRidley(Sprite* sprite_boss, InfoSprite* info_boss);
	void NewBullet(D3DXVECTOR2 Position, bool flipflag);
	void OnCollision(Object *obj, D3DXVECTOR2 distance);
	void ChangeAnimation(Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void SetPosStart(D3DXVECTOR2 pos)
	{
		posStart = pos;
		position = posStart;
	}
};
