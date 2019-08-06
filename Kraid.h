#pragma once
#pragma once
#include"Boss.h"
#include"Bumerang.h"
#include"BulletKraid.h"
class Kraid :public Boss
{
protected:
	bool isJump;
public:
	Bumerang* mlistBumerang[2];
	BulletKraid* mlistBullet[3];
	
	Kraid() {}
	~Kraid()
	{
		for (int i = 0; i < 2; i++)
		{
			Bumerang *bullet = mlistBumerang[i];
			delete bullet;
		}
		for (int i = 0; i < 3; i++)
		{
			BulletKraid *bullet = mlistBullet[i];
			delete bullet;
		}
	}
	Kraid(Sprite* sprite_boss, InfoSprite* info_boss, D3DXVECTOR2 pos);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void OnCollision(Object *obj, D3DXVECTOR2 distance);
	void OnCollision(Object *obj);
	void OnCollision();
	void ChangeAnimation(Keyboard* key);
	void New();
};