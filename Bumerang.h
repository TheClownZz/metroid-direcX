#pragma once
#include "Object.h"
#include "Animation.h"
#include "GameDefine.h"
#include"Collision.h"

using namespace Define;

class Bumerang : public Object
{
protected:
	Animation* mAnimation;
	D3DXVECTOR2 posStart;

public:
	bool isShoot;
	Bumerang() {};
	Bumerang(Sprite* sprite_boss, InfoSprite* info_boss);
	~Bumerang()
	{
		delete mAnimation;
	} //
	void Shoot(bool flipflap);
	void NewBullet(D3DXVECTOR2 Position);
	void OnCollision();
	void ChangeAnimation(Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void SetPosStart(D3DXVECTOR2 pos)
	{
		posStart = pos;
		position = posStart;
	}
};
