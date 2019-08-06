#include"BulletKraid.h"

BulletKraid::BulletKraid(Sprite* sprite_boss, InfoSprite* info_boss)
{
	sprite = sprite_boss;
	mAnimation = new Animation(info_boss);

	position = D3DXVECTOR2(0, 0);
	velocity = D3DXVECTOR2(0, 0);
	transform = D3DXVECTOR2(0, 0);
	Tag = Object::tag::bullet;
	posStart = position;
	
	isShoot = false;
	delay = 0;
	GetBound(6, 3);
}
void BulletKraid::NewBullet(D3DXVECTOR2 pos)
{
	posStart = pos;
	position = posStart;
}

void BulletKraid::OnCollision(Object *obj)
{
	if (obj->GetDamage() == 2)
	{
		Frezz = true;
		delay = 500;
	}
}

void BulletKraid::Shoot(bool flipflap)
{
	FlipFlag = flipflap;
	if (isShoot)
		return;
	if (flipflap)
		SetVelocityX(0.5);
	else
		SetVelocityX(-0.5);
	isShoot = true;
}
void BulletKraid::ChangeAnimation(Keyboard* key)
{
	GetBound(6, 3);
	if (Frezz)
	{
		Damage = 0;
		mAnimation->SetFrame(position, FlipFlag, 0, 19, 19);
	}
	else
	{
		Damage = 10;
		mAnimation->SetFrame(position, FlipFlag, 0, 18, 18);
	}
}
void BulletKraid::Update(float gameTime, Keyboard* key)
{
	ChangeAnimation(key);
	mAnimation->Update(gameTime, key);
	delay--;
	if (delay < 0)
	{
		Frezz = false;
		Object::Update(gameTime, key);
		if (abs(position.x - posStart.x) > 100)
		{
			isShoot = false;
			velocity.x = 0;
		}
	}
}
void BulletKraid::Render(Viewport* viewport)
{
//	rectlinebound->DrawRect(GetBound(), viewport);
	sprite->SetData(
		mAnimation->GetRect(),
		mAnimation->GetCenter(),
		mAnimation->GetPosition(),
		mAnimation->GetScale(),
		mAnimation->GetTransform(),
		mAnimation->GetAngle()
	);
	sprite->Render(viewport);
}