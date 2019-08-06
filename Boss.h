#pragma once
#include "Animation.h"
#include "GameDefine.h"
#include"Collision.h"
#include"Bullet.h"
#include"SamusAran.h"
using namespace Define;
class Boss : public Object
{
protected:
	int delay;
	Animation *AnimationBoss;
public:
	RECT sceneboss;
	enum StateBoss
	{
		Moving,
		Dying,
		Jumping,
		Hitted,
		JumpingHitted
	}mState;
	Boss(){}
	~Boss()
	{
		delete AnimationBoss;
	}
	virtual void Update(float gameTime, Keyboard* key);
	virtual void Render(Viewport* viewport);
	virtual void OnCollision(Object *obj, D3DXVECTOR2 distance);
	virtual void OnCollision(Object *obj);
	virtual void OnCollision();
	virtual void ChangeAnimation(Keyboard* key);

};