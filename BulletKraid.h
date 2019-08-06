#pragma once
#include "Object.h"
#include "Animation.h"
#include "GameDefine.h"
#include"Collision.h"

using namespace Define;

class BulletKraid : public Object
{
protected:
	Animation* mAnimation;
	D3DXVECTOR2 posStart;
	
	bool Frezz;
	int delay;
public:
	bool isShoot;
	BulletKraid() {};
	BulletKraid(Sprite* sprite_boss, InfoSprite* info_boss);
	~BulletKraid()
	{
		delete mAnimation;
	}
	void Shoot(bool flipflap);
	void NewBullet(D3DXVECTOR2 Position);
	void OnCollision(Object *obj);
	void ChangeAnimation(Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void SetPosStart(D3DXVECTOR2 pos)
	{
		posStart = pos;
		position = posStart;
	}
};
