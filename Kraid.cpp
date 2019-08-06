#include"Kraid.h"
Kraid::Kraid(Sprite* sprite_boss, InfoSprite* info_boss, D3DXVECTOR2 pos)
{
	positionStart = pos;
	transform = D3DXVECTOR2(0, 0);
	this->sprite = sprite_boss;
	this->AnimationBoss = new Animation(info_boss);
	Tag = tag::Enemy;
	New();
	for (int i = 0; i < 2; i++)
		mlistBumerang[i] = new Bumerang(sprite_boss, info_boss);
	for (int i = 0; i < 3; i++)
		mlistBullet[i] = new BulletKraid(sprite_boss,info_boss);
	
}

void Kraid::New()
{
	position = positionStart;
	GetBound(20, 40);
	isJump = true;
	AllowDraw = false;
	FlipFlag = false;
	mState = Moving;
	delay = 120;
	SetVelocity(0.25, -Gravity);
	HP = 100;
	Damage = 10;
	sceneboss = { 3712, 1440, 3968, 1200 };
}

void Kraid::ChangeAnimation(Keyboard* key)
{
	GetBound(20, 40);
	switch (mState)
	{
	case Moving:
		AnimationBoss->SetFrame(position, FlipFlag, 4, 0, 1);
		break;
	case Dying: 
	{
		bound = { 0,0,0,0 };
		SetVelocity(0.0f, -0.5f);
		AnimationBoss->SetFrame(position, FlipFlag, 2, 32, 33);
		break;
	}
	case Hitted:
		AnimationBoss->SetFrame(position, FlipFlag, 1, 2, 3);
		break;
	}

}
void Kraid::Update(float gameTime, Keyboard* key)
{
	if (!AllowDraw)
		return;

	ChangeAnimation(key);
	AnimationBoss->Update(gameTime, key);

	if (mState == Dying)
	{
		SetVelocity(0, 0);
		delay--;
		if (delay < 0)
			sceneboss = { 0, 0, 0, 0 };

	}
	else
	{
		Object::Update(gameTime, key);
		if (abs(position.x - positionStart.x) > 30)
			SetVelocityX(-velocity.x);

		if (delay <= 0)
			delay = 120;
		delay--;
		for (int i = 0; i < 2; i++)
		{
			mlistBumerang[i]->Update(gameTime, key);
			if (!mlistBumerang[i]->isShoot)
			{
				if (FlipFlag)
					mlistBumerang[i]->NewBullet(
						D3DXVECTOR2(position.x - Width / 2, position.y + Height / 2 - (2 - i)*mlistBumerang[i]->GetHeight()));
				else
					mlistBumerang[i]->NewBullet(
						D3DXVECTOR2(position.x + Width / 2, position.y + Height / 2 - (2 - i)*mlistBumerang[i]->GetHeight()));

			}
			if (delay == 60 * i)
				mlistBumerang[i]->Shoot(FlipFlag);
		}
		for (int i = 0; i < 3; i++)
		{
			mlistBullet[i]->Update(gameTime, key);
			if (!mlistBullet[i]->isShoot)
			{
				if (FlipFlag)
					mlistBullet[i]->NewBullet(
						D3DXVECTOR2(position.x + Width / 2, position.y + (1 - i) *Height / 2 + (i - 1)*mlistBullet[i]->GetHeight() ));
				else
					mlistBullet[i]->NewBullet(
						D3DXVECTOR2(position.x - Width / 2, position.y + (1 - i) *Height / 2 + (i - 1)*mlistBullet[i]->GetHeight() ));

			}
			if (delay == 40 * i)
				mlistBullet[i]->Shoot(FlipFlag);
		}
	}
		
}
void Kraid::OnCollision(Object *obj, D3DXVECTOR2 distance)
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
				position.x += distance.x*Time;
			else if (SideCollision.y != 0)
				position.y += distance.y*Time;
			if (SideCollision.y == 1)
			{
				velocity.y = 0;
			}
		}
	}
}
void Kraid::OnCollision(Object *obj)
{
	if (obj->Tag == Object::bullet)
	{
		//SetVelocity(0.0f, 0.0f);
		HP -= obj->GetDamage();
		if (HP <= 0)
		{
			delay = 100;
			mState = Dying;
			SetVelocity(0, 0);
		}
		return;
	}
	else if (obj->Tag == Object::player)
	{
		if (position.x <= obj->GetPosition().x)
			FlipFlag = true;
		else
			FlipFlag = false;
	}
	
}
void Kraid::OnCollision()
{

}
void Kraid::Render(Viewport* viewport)
{
	if (!AllowDraw)
		return;

	this->sprite->SetData(
		AnimationBoss->GetRect(),
		AnimationBoss->GetCenter(),
		AnimationBoss->GetPosition(),
		AnimationBoss->GetScale(),
		AnimationBoss->GetTransform(),
		AnimationBoss->GetAngle()
	);
	this->sprite->Render(viewport);

	for (int i = 0; i < 2; i++)
		mlistBumerang[i]->Render(viewport);
	for (int i = 0; i < 3; i++)
		mlistBullet[i]->Render(viewport);
}

