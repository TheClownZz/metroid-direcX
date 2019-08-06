#include"Ridley.h"
Ridley::Ridley(Sprite* sprite_boss, InfoSprite* info_boss, D3DXVECTOR2 pos)
{
	positionStart = pos;
	this->sprite = sprite_boss;
	this->AnimationBoss = new Animation(info_boss);
	transform = D3DXVECTOR2(0, 0);
	Tag = tag::Enemy;
	New();
	for (int i = 0; i < 6; i++)
		mListBullet[i] = new BulletRidley(sprite_boss,info_boss);
}

void Ridley::New()
{
	position = positionStart;
	GetBound(20, 40);
	isJump = true;
	AllowDraw = false;
	FlipFlag = false;
	mState = Moving;
	delay = 40;
	indexBullet = 0;
	HP = 100;
	Damage = 10;
	sceneboss = { 3200, 1440, 3456, 1200 };
}

void Ridley::ChangeAnimation(Keyboard* key)
{
	GetBound(20, 40);
	switch (mState)
	{
	case Moving:
		AnimationBoss->SetFrame(position, FlipFlag, 3, 6, 7);
		break;
	case Jumping:
		AnimationBoss->SetFrame(position, FlipFlag, 4, 8, 9);
		break;
	case Hitted:
	{
		AnimationBoss->SetFrame(position, FlipFlag, 4, 28, 29);
		if (AnimationBoss->GetIndex() == 29)
			mState = Moving;
		break;
	}
	case JumpingHitted:
	{
		AnimationBoss->SetFrame(position, FlipFlag, 4, 30, 31);
		if (AnimationBoss->GetIndex() == 31)
			mState = Jumping;
		break;
	}
	case Dying:
	{
		bound = { 0,0,0,0 };
		AnimationBoss->SetFrame(position, FlipFlag, 4, 32, 33);
		break;
	}
	}
}
void Ridley::Update(float gameTime, Keyboard* key)
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
			sceneboss = {0, 0, 0, 0};

	}
	else
	{
		Object::Update(gameTime, key);

		if (!isJump)
		{
			if (delay <= 0)
				delay = 40;
			delay--;
			if (delay <= 0)
			{
				mState = Jumping;
				SetVelocityY(2.5);
				isJump = true;
				if (indexBullet >= 6)
					indexBullet = 0;
				mListBullet[indexBullet]->NewBullet(D3DXVECTOR2(position.x, position.y + Height), FlipFlag);
				indexBullet++;
			}
		}
		else
			AddVelocity(0, -0.1);
		for (int i = 0; i < 6; i++)
			mListBullet[i]->Update(gameTime, key);
	}
}
void Ridley::OnCollision(Object *obj, D3DXVECTOR2 distance)
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
				mState = Moving;
				if (indexBullet >= 6)
					indexBullet = 0;
				mListBullet[indexBullet]->NewBullet(D3DXVECTOR2(position.x, position.y + Height), GetFlipFlag());
				indexBullet++;
				isJump = false;
				SetVelocityY(0);
			}
		}
	}
}
void Ridley::OnCollision(Object *obj)
{
	if (obj->Tag == Object::bullet)
	{
		if (mState == Moving)
			mState == Hitted;
		else if (mState == Jumping)
			mState == JumpingHitted;

		HP -= obj->GetDamage();
		if (HP <= 0)
		{
			delay = 100;
			velocity.y = 0;
			mState = Dying;
			isJump = false;
		}
		return;
	}
	if (obj->Tag == Object::player)
	{
		if (position.x <= obj->GetPosition().x)
			FlipFlag = false;
		else
			FlipFlag = true;
	}
	
}
void Ridley::OnCollision()
{
	
}
void Ridley::Render(Viewport* viewport)
{
	if (!AllowDraw)
		return;

	sprite->SetData(
		AnimationBoss->GetRect(),
		AnimationBoss->GetCenter(),
		AnimationBoss->GetPosition(),
		AnimationBoss->GetScale(),
		AnimationBoss->GetTransform(),
		AnimationBoss->GetAngle()
	);
	sprite->Render(viewport);

	for (int i = 0; i < 6; i++)
		mListBullet[i]->Render(viewport);
}

