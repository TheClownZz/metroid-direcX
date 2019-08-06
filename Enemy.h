#pragma once
#include "Animation.h"
#include "SamusAran.h"
#include "Items.h"
#include "GameDefine.h"
#include"Collision.h"
using namespace Define;

class Enemy : public Object
{
protected:
	Animation * AnimationEnemy;
	D3DXVECTOR2 velocityold, side;
	SamusAran *Samus;
	Items *Item;
	int delay, delayMove, frameEnemy, indexFrezz;
	float timedie;
	bool isSeePlayer, change;
	bool KillYourSeft;
	Bullet* bullet_skree[4];
	Sprite* sprite_Bullet;
	InfoSprite* info_bullet;
public:
	enum Enemytype
	{
		Zoomer,//Con nhím
		Skree,//Con chuồn chuồn
		Ripper,//Con gì nó qua lại
		Rio,//Con 20 máu
		Waver,//Con bay lên xuống
		Zeb//Con gì từ cái ống bay ra

	};
	Enemytype EnemyType;

	enum Stateenemy
	{
		Moving,
		ZoomerMoveLeft,
		ZoomerMoveRight,
		ZoomerMoveTop,
		Frezing,
		Dying
	};
	int level;
	Stateenemy StateEnemy;
	Enemy();
	Enemy(SamusAran *Samus, Sprite* sprite_enemy, Sprite* sprite_item, Sprite* sprite_bullet, InfoSprite* info_bullet, InfoSprite* info_enemy, InfoSprite* info_item, Sound* sound);
	~Enemy();
	Enemytype GetEnemyType();
	void SetEnemyType();
	Stateenemy GetStateEnemy();
	void SetStateEnemy(Stateenemy stateenemy);
	Enemytype GetType(string name);
	void NewEnemy(D3DXVECTOR2 pos, Enemytype type);
	//void OnCollision();
	void ChangeAnimation(Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void OnCollision(Object *obj, D3DXVECTOR2 distance);
	void OnCollision(Object *obj);
	void OnCollision();
};

