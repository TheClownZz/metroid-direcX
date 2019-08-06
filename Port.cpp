#include "Port.h"

Port::Port()
{
}

Port::Port(InfoMap *Info, SamusAran *samus, Sprite* sprite_port, InfoSprite*info_port, Sound* sound)
{
	//sound
	this->sound = sound;
	soundportopen = sound->LoadSound(SoundPortCollison);
	soundportclose = sound->LoadSound(SoundPortCollison);
	Tag = tag::Port;
	sprite = sprite_port;
	AnimationPort = new Animation(info_port);
	info = Info;
	Samus = samus;
	data = 0;
	AllowDraw = false;
	Open = false;
	timeclose = 0.0f;
	SceneMove = false;
	BoundMove = { 0,0,0,0 };
	transform = D3DXVECTOR2(0, 0);
}

Port::~Port()
{
	delete sprite;
	delete AnimationPort;

	delete sound;
	delete soundportopen;
	delete soundportclose;
}
void Port::New()
{
	AllowDraw = true;
	Open = false;
	timeclose = 0.0f;
	HP = 5;
	GetBound(48, 48);
}
//Tạo port mới
void Port::NewPort(D3DXVECTOR2 pos, Porttype type)
{
	AllowDraw = true;
	Open = false;
	position = pos;
	PortType = type;
	timeclose = 0.0f;
	switch (PortType)
	{
	case Port::PortRock:
	{
		data = info->GetData(pos);
		GetBound(16, 16);
		break;
	}
	case Port::PortYellow:
	{
		slip = false;
		HP = 5;
		BoundMove = GetBound(32, 48);
		GetBound(48, 48);
		break;
	}
	case Port::PortBlue:
	{
		slip = false;
		Samus->LockAnimation(false, -1);
		BoundMove = GetBound(32, 48);
		GetBound(48, 48);
		break;
	}
	default:
		GetBound(16, 16);
		break;
	}
}
Port::Porttype Port::GetType(string name)
{
	if (name == "PortItem")
		return PortItem;
	if (name == "PortRock")
		return PortRock;
	if (name == "PortYellow")
		return PortYellow;
	if (name == "PortBlue")
		return PortBlue;
}

//Chuyển Animation
void Port::ChangeAnimation(Keyboard* key)
{
	switch (PortType)
	{
	case Port::PortItem:
	{
		//Nếu Cổng đang mở thì mở xong tắt vẽ
		if (Open)
		{
			if (AnimationPort->GetIndex() == 2)
			{
				AllowDraw = false;
				return;
			}
			AnimationPort->SetFrame(position, false, PortDelay, 0, 2);
		}
		else
			AnimationPort->SetFrame(position, false, PortDelay, 0, 0);
		break;

	}
	case Port::PortRock:
	{
		if (Open)
		{
			if (position.x < 3000)
			{
				if (AnimationPort->GetIndex() == 2)
				{
					AnimationPort->SetIndex(1);
					AllowDraw = false;
					return;
				}
				AnimationPort->SetFrame(position, false, PortDelay, 1, 2);
			}
			else
			{
				if (AnimationPort->GetIndex() == 5)
				{
					AnimationPort->SetIndex(4);
					AllowDraw = false;
					return;
				}
				AnimationPort->SetFrame(position, false, PortDelay, 4, 5);
			}
		}
		else
			AllowDraw = false;
		break;
	}
	//Cổng xanh
	case Port::PortBlue:
	{
		if (position.x > Samus->GetPosition().x)
			FlipFlag = false;
		else
			FlipFlag = true;
		//Chuyển Scene khi vào vùng chuyển thì đóng cổng
		if (!slip && Collision::isCollision1(Samus->GetBound(), BoundMove) )
		{
			sound->PlaySoundA(soundportclose);

			slip = true;
			GetBound(48, 48);
			Open = false;
			AllowDraw = true;
			Samus->LockAnimation(true, -1);
			timeclose = 0.0f;
		}

		//Khi Samus đang trượt
		if (slip)
		{
			//Samus bên phải cổng
			if (Samus->GetFlipFlag())
			{
				//Khi scene di chuyển thì Samus đứng yên và ngược lại
				if (Samus->GetPosition().x < position.x && SceneMove)
					Samus->SetVelocityX(0.0f);
				else
					Samus->SetVelocityX(-0.5f);
				//Ra khỏi cổng thì đóng lại
				if (Samus->GetBound().right < position.x - 25)
				{
					Samus->LockAnimation(false, -1);
					sound->PlaySoundA(soundportclose);
					NewPort(position, PortType);
				}
				else if (Samus->GetBound().right < position.x && !Open)
				{
					//Mở khi gần ra
					sound->PlaySoundA(soundportopen);

					AnimationPort->SetPause(false);
					Open = true;
					AllowDraw = true;
				}
			}
			//Samus bên trái cổng
			else
			{
				//Khi scene di chuyển thì Samus đứng yên và ngược lại
				if (Samus->GetPosition().x > position.x && SceneMove)
					Samus->SetVelocityX(0.0f);
				else
					Samus->SetVelocityX(0.5f);
				//Ra khỏi cổng thì new port
				if (Samus->GetBound().left > position.x + 25)
				{
					Samus->LockAnimation(false, -1);
					sound->PlaySoundA(soundportclose);
					NewPort(position, PortType);
				}
				else if (Samus->GetBound().left > position.x && !Open)
				{
					sound->PlaySoundA(soundportopen);

					AnimationPort->SetPause(false);
					Open = true;
					AllowDraw = true;
				}
			}
		}

		//Khi cổng đóng mở
		if (Open)
		{
			if (AnimationPort->GetIndex() == 8)
			{
				AnimationPort->SetIndex(7);
				AllowDraw = false;
				return;
			}
			AnimationPort->SetFrame(position, FlipFlag, PortDelay, 6, 8);
		}
		else
		{
			if (AnimationPort->GetIndex() == 8)
			{
				AnimationPort->SetPause(true, 6);
				return;
			}
			AnimationPort->SetFrame(position, FlipFlag, PortDelay, 7, 8);
			
		}
		break;
	}
	case Port::PortYellow:
	{
		//Chuyển Scene khi vào vùng chuyển
		if (Collision::isCollision1(Samus->GetBound(), BoundMove))
		{
			Samus->LockAnimation(true, -1);
			//Samus bên phải cổng
			if (Samus->GetFlipFlag())
			{
				BoundMove.left = position.x - 25;
				if (Samus->GetPosition().x < position.x && SceneMove)
					Samus->SetVelocityX(0.0f);
				else
					Samus->SetVelocityX(-RunSpeed);

				if (Samus->GetBound().right < position.x - 16)
				{
					Samus->LockAnimation(false, -1);
					BoundMove.left = position.x - 16;
				}
			}
			//Samus bên trái cổng
			else
			{
				BoundMove.right = position.x + 25;
				if (Samus->GetPosition().x > position.x && SceneMove)
					Samus->SetVelocityX(0.0f);
				else
					Samus->SetVelocityX(RunSpeed);

				if (Samus->GetBound().left > position.x + 16)
				{
					Samus->LockAnimation(false, -1);
					BoundMove.right = position.x + 16;
				}
			}
		}

		if (Open)
		{
			if (AnimationPort->GetIndex() == 11)
			{
				AnimationPort->SetIndex(10);
				AllowDraw = false;
				return;
			}
			AnimationPort->SetFrame(position, false, PortDelay, 9, 11);
		}
		else
		{
			if (AnimationPort->GetIndex() == 11)
			{
				AnimationPort->SetPause(true, 9);
				return;
			}
			AnimationPort->SetFrame(position, false, PortDelay, 10, 11);
			
		}
		break;
	}
	
	default:
		break;
	}
}

//Khi đạn chạm
void  Port::OnCollision()
{
	if (PortType == Port::PortYellow)
	{
		if (Samus->bullet[1]->GetBulletType() == Bullet::RocketBullet)
		{
			HP--;
			if (HP <= 0)
			{
				AnimationPort->SetPause(false);
				Open = true;
				bound = { 0, 0, 0, 0 };
			}
		}
	}
	else
	{
		if (PortType == Port::PortBlue)
			sound->PlaySoundA(soundportopen);

		AnimationPort->SetPause(false);
		AllowDraw = true;
		Open = true;
		bound = { 0, 0, 0, 0 };

		if (PortType == Port::PortRock)
		{
			info->SetData(position, 0);
		}
	}
}

void Port::Update(float gameTime, Keyboard *key)
{
	if (Open)
	{
		timeclose += gameTime;
		if (timeclose >= 3.0f && PortType != Port::PortYellow)
		{
			if (PortType == Port::PortRock)
			{
				info->SetData(position, data);
			}
			NewPort(position, PortType);
			return;
		}
	}

	ChangeAnimation(key);
	AnimationPort->Update(gameTime, key);
}

void Port::Render(Viewport *view)
{
	if (position.x == view->GetScene().left || position.x == view->GetScene().right)
		SceneMove = false;
	else 
		SceneMove = true;
	//Vẽ Port
	if (AllowDraw)
	{
		sprite->SetData(
			AnimationPort->GetRect(),
			AnimationPort->GetCenter(),
			AnimationPort->GetPosition(),
			AnimationPort->GetScale(),
			AnimationPort->GetTransform(),
			AnimationPort->GetAngle()
		);
		sprite->Render(view);
	}
}