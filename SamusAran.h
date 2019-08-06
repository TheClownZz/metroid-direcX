#pragma once
#include "Object.h"
#include "SamusAranAnimation.h"
#include "SamusAranState.h"
#include "Bullet.h"
#include "SamusAranCollision.h"
#include "GameDefine.h"

using namespace Define;

class SamusAran : public Object
{
protected:
	SamusAranAnimation*	AnimationSamus;
	SamusAranState* StateSamus;
	SamusAranCollision* CollisionSamus;

	SamusAranState::Statename StateName;
	//Thùng Đạn
	int bulletorder, boomorder;
	bool checkupshoot, changeGun, RocketGun;

	//Sound
	GSound *soundrun;
	GSound *soundjump;
	GSound *soundSamusCollision;
	GSound *sounddying;
	GSound *sounddie;
	int countdie;
	float posxdie, posydie;
public:
	SamusAran() {}
	SamusAran(Sprite* sprite_samus, Sprite* sprite_bullet, InfoSprite* info_bullet,Sound *sound);
	~SamusAran();
	const char* pathPNG;
	bool changevaria;
	Bullet* bullet[10];
	float SBullet;
	int rocketbullet, MaxRocket, Tank;
	InfoSprite* info_samus;
	struct Flagitems
	{
		bool maru = false;
		bool boom = false;
		bool rocket = false;
		bool icebean = false;
		bool varia = false;
	} FlagItems;

	RECT GetBoard(D3DXVECTOR2 distance);
	void New();
	void LockAnimation(bool Lock, int index);
	void ChangeGun(Keyboard* key, bool fire);
	void ChangeAnimation(float gameTime, Keyboard* key);
	void SetBleedState(int side, int dame, bool Bleed);
	void OnCollision(Object *obj, D3DXVECTOR2 distance, D3DXVECTOR2 disSamus);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);

};

