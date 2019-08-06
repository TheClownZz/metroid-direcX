#pragma once
#include "Object.h"
#include "Animation.h"
#include "GameDefine.h"
#include"Collision.h"
using namespace Define;

class Bullet : public Object
{
protected:
	GSound* soundfire;
	GSound* soundicefire;
	GSound* soundboomfire;
	GSound* soundrocketfire;
	GSound* soundboomburst;

	Animation* AnimationBullet;
	
	float TimeBulletFireMax, timefly;

public:
	enum Bullettype
	{
		NormalBullet,
		RocketBullet,
		IceBullet,
		Boom
	};
	Bullettype BulletType;

	enum Statebullet
	{
		Firing,
		Bursting,
	};
	Statebullet StateBullet;

	Bullet();
	Bullet(Sprite* sprite_bullet, InfoSprite* info_bullet, Sound *sound);
	~Bullet();

	Bullettype GetBulletType();
	void SetBulletType(Bullettype bullettype);
	Statebullet GetStateBullet();
	void SetStateBullet(Statebullet statebullet);

	void NewBullet(D3DXVECTOR2 Position, bool flipflag, bool shootup, float Time);
	void OnCollision();

	void ChangeAnimation(Keyboard* key);
	static Bullettype GetType(string name);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
};
