#include"Bumerang.h"

Bumerang::Bumerang(Sprite* sprite_boss, InfoSprite* info_boss)
{
	sprite = sprite_boss;
	mAnimation = new Animation(info_boss);
	position = D3DXVECTOR2(0, 0);
	velocity = D3DXVECTOR2(0, 0);
	transform = D3DXVECTOR2(0, 0);
	Tag = Object::tag::bullet;
	posStart = position;
	isShoot = false;
	GetBound(6, 6);

	Damage = 10;
}
void Bumerang::NewBullet(D3DXVECTOR2 pos)
{
	posStart = pos;
	position = posStart;
}
void Bumerang::OnCollision()
{

}
void Bumerang::Shoot(bool flipflap)
{
	FlipFlag = flipflap;
	if (isShoot)
		return;
	if (flipflap)
		SetVelocity(0.5, 2.5);
	else
		SetVelocity(-0.5, 2.5);
	isShoot = true;
}
void Bumerang::ChangeAnimation(Keyboard* key)
{
	GetBound(6,6);
	mAnimation->SetFrame(position, FlipFlag, 3, 10, 13);
}
void Bumerang::Update(float gameTime, Keyboard* key)
{
	ChangeAnimation(key);
	Object::Update(gameTime, key);
	mAnimation->Update(gameTime, key);

	if (abs(position.x - posStart.x) > 100)
	{
		isShoot = false;
		SetVelocity(0, 0);
	}
	if (velocity.y!=0)
		AddVelocity(0, -0.05);	
}
void Bumerang::Render(Viewport* viewport)
{
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