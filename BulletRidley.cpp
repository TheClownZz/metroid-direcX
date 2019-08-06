#include"BulletRidley.h"

BulletRidley::BulletRidley(Sprite* sprite_boss, InfoSprite* info_boss)
{
	sprite = sprite_boss;
	mAnimation = new Animation(info_boss);

	position = D3DXVECTOR2(0, 0);
	velocity = D3DXVECTOR2(0, 0);
	transform = D3DXVECTOR2(0, 0);
	Tag = Object::tag::bullet;
	posStart = position;

	AllowDraw = false;
	GetBound(8, 8);

	Damage = 10;
}
void BulletRidley::NewBullet(D3DXVECTOR2 pos, bool flipflag)
{
	FlipFlag = flipflag;
	posStart = pos;
	position = posStart;
	if (!flipflag)
		SetVelocity(0.35,2);
	else
		SetVelocity(-0.35,2);
	AllowDraw = true;
}
void BulletRidley::OnCollision(Object *obj, D3DXVECTOR2 distance)
{
	RECT board = this->GetBoard(distance);
	//Nếu other trong vùng di chuyển
	if (Collision::isCollision(board, obj->GetBound()))
	{
		//Lấy cạnh va chạm
		D3DXVECTOR2 SideCollision;
		//lấy thời gian va chạm
		float Time = Collision::CollisionAABB(GetBound(), obj->GetBound(), distance, SideCollision);
		//bé hơn 1 thì có va chạm
		if (Time < 1.0f)
		{
			if (SideCollision.x != 0)
			{
				position.x += distance.x*Time;
				AllowDraw = false;
				bound = { 0, 0, 0, 0 };
			}
			else if (SideCollision.y != 0)
				position.y += distance.y*Time;

			if (SideCollision.y == 1)
			{
				velocity.y = -velocity.y/1.1;
			}
		}
	}
}
void BulletRidley::ChangeAnimation(Keyboard* key)
{
	GetBound(6, 6);
	mAnimation->SetFrame(position, FlipFlag, BulletDelay, 20, 23);
}
void BulletRidley::Update(float gameTime, Keyboard* key)
{
	if (!AllowDraw)
		return;
	ChangeAnimation(key);
	Object::Update(gameTime, key);
	mAnimation->Update(gameTime, key);

	if (abs(position.x - posStart.x) > 100)
	{
		AllowDraw = false;
		bound = { 0, 0, 0, 0 };
	}
	AddVelocity(0, -0.1);
}
void BulletRidley::Render(Viewport* viewport)
{
	if (!AllowDraw)
		return;
	//rectlinebound->DrawRect(GetBound(), viewport);
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