#include "Bullet.h"



Bullet::Bullet()
{

}

Bullet::Bullet(Sprite* sprite_bullet, InfoSprite* info_bullet, Sound *sound)
{
	this->sound = sound;

	soundfire = sound->LoadSound(SoundFire);
	soundicefire = sound->LoadSound(SoundIceFire);
	soundboomfire = sound->LoadSound(SoundBoomFire);
	soundrocketfire = sound->LoadSound(SoundRocketFire);
	soundboomburst = sound->LoadSound(SoundBoomBurst);

	sprite = sprite_bullet;
	AnimationBullet = new Animation(info_bullet);
	AllowDraw = false;

	timefly = 0;
	TimeBulletFireMax = 0;
	BulletType = NormalBullet;

	position = D3DXVECTOR2(0, 0);
	velocity = D3DXVECTOR2(0, 0);
	transform = D3DXVECTOR2(0, 0);

	Tag = Object::tag::bullet;
}

Bullet::~Bullet()
{
	delete soundfire;
	delete soundicefire;
	delete soundboomfire;
	delete soundrocketfire;
	delete soundboomburst;

	delete AnimationBullet;
}

//Tạo thông số cho viên đạn khi bắn
void Bullet::NewBullet(D3DXVECTOR2 Position, bool flipflag, bool shootup, float time)
{
	switch (BulletType)
	{
	case Bullet::NormalBullet:
	{
		Damage = 1;
		sound->PlaySoundA(soundfire);
		break;
	}
	case Bullet::RocketBullet:
	{
		Damage = 6;
		sound->PlaySoundA(soundrocketfire);
		break;
	}
	case Bullet::IceBullet:
	{
		Damage = 2;
		sound->PlaySoundA(soundicefire);
		break;
	}
	case Bullet::Boom:
	{
		Damage = 5;
		sound->PlaySoundA(soundboomfire);
		break;
	}
	default:
		break;
	}

	this->AllowDraw = true;
	this->position = Position;

	this->FlipFlag = flipflag;
	StateBullet = Firing;
	if (BulletType == Boom)
	{
		SetVelocity(0, 0);
	}
	else
	{
		if (shootup)
		{
			//Lấy góc để vẽ animation viên đạn bên trái thì quay 90 phải -90
			if (flipflag)
				AnimationBullet->SetAngle(90.0f);
			else
				AnimationBullet->SetAngle(-90.0f);

			SetVelocity(0, BulletSpeed);
		}
		else
		{
			AnimationBullet->SetAngle(0.0f);

			if (flipflag)
				SetVelocity(-BulletSpeed, 0);
			else
				SetVelocity(BulletSpeed, 0);
		}
	}
	TimeBulletFireMax = time;
	timefly = 0;
}

Bullet::Bullettype Bullet::GetBulletType()
{
	return BulletType;
}

void Bullet::SetBulletType(Bullet::Bullettype bullettype)
{
	BulletType = bullettype;
}

Bullet::Statebullet Bullet::GetStateBullet()
{
	return StateBullet;
}

void Bullet::SetStateBullet(Statebullet statebullet)
{
	StateBullet = statebullet;
}


void Bullet::ChangeAnimation(Keyboard* key)
{
	switch (BulletType)
	{
	case Bullet::NormalBullet:
	{
		switch (StateBullet)
		{
		case Bullet::Firing:
		{
			GetBound(4, 4);
			AnimationBullet->SetFrame(position, FlipFlag, 0, 0, 0);
			break;
		}
		case Bullet::Bursting:
			AnimationBullet->SetFrame(position, FlipFlag, BulletDelay, 1, 3);
			break;
		}
		break;
	}

	case Bullet::IceBullet:
	{
		switch (StateBullet)
		{
		case Bullet::Firing:
		{
			GetBound(4, 4);
			AnimationBullet->SetFrame(position, FlipFlag, 0, 4, 4);
			break;
		}
		case Bullet::Bursting:
			AnimationBullet->SetFrame(position, FlipFlag, BulletDelay, 5, 7);
			break;
		}
		break;
	}

	case Bullet::Boom:
	{
		switch (StateBullet)
		{
		case Bullet::Firing:
		{
			GetBound(8, 8);
			AnimationBullet->SetFrame(position, FlipFlag, BulletDelay, 8, 9);
			break;
		}
		case Bullet::Bursting:
		{
			AnimationBullet->SetFrame(position, FlipFlag, BulletDelay, 10, 12);
			break;
		}
		}
		break;
	}

	case Bullet::RocketBullet:
	{
		switch (StateBullet)
		{
		case Bullet::Firing:
		{
			GetBound(5, 5);
			AnimationBullet->SetFrame(position, FlipFlag, 0, 13, 13);
			break;
		}
		case Bullet::Bursting:
			AnimationBullet->SetFrame(position, FlipFlag, BulletDelay, 14, 15);
			break;
		}
		break;
	}

	}
}

void Bullet::Update(float gameTime, Keyboard* key)
{
	if (AllowDraw)
	{
		ChangeAnimation(key);
		Object::Update(gameTime, key);

		AnimationBullet->Update(gameTime, key);
	}

	timefly += gameTime;
	if (BulletType == Boom)
	{
		if (TimeBulletFireMax - timefly <= 0.35f)
			GetBound(16, 16);

		if (TimeBulletFireMax - timefly <= 0.3f)
		{
			sound->PlaySoundA(soundboomburst);
			StateBullet = Bullet::Statebullet::Bursting;
		}
	}

	if (timefly >= TimeBulletFireMax)
	{
		timefly = 0;
		AllowDraw = false;
		SetVelocity(0, 0);
		bound = { 0, 0, 0, 0 };
	}

}

void Bullet::Render(Viewport* viewport)
{
	//Vẽ đạn
	if (AllowDraw)
	{
		
		if (Collision::isCollision(position.x, position.y, viewport->GetBoundViewport()))
		{
			sprite->SetData(
				AnimationBullet->GetRect(),
				AnimationBullet->GetCenter(),
				AnimationBullet->GetPosition(),
				AnimationBullet->GetScale(),
				AnimationBullet->GetTransform(),
				AnimationBullet->GetAngle()
			);
			sprite->Render(viewport);
		}
		else
		{
			bound = { 0, 0, 0, 0 };
			AllowDraw = false;
		}
	}
}
void Bullet::OnCollision()
{
	//Nếu other trong vùng di chuyển
		TimeBulletFireMax = timefly + 0.2f;
		StateBullet = Bullet::Statebullet::Bursting;
		this->SetVelocity(0, 0);
		bound = { 0, 0, 0, 0 };
}

Bullet::Bullettype Bullet:: GetType(string name)
{
	if (name == "NormalBullet")
		return NormalBullet;
	if (name == "RocketBullet")
		return RocketBullet;
	if (name == "IceBullet")
		return IceBullet;
	if (name == "Boom")
		return Boom;
}