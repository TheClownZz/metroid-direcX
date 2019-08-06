#include "Items.h"

Items::Items()
{
}

Items::Items(SamusAran *Samus, Sprite* _sprite, InfoSprite* info_item, Sound* sound)
{
	this->sound = sound;
	sounditemHP = sound->LoadSound(SoundItemHP);
	sounditemrocket = sound->LoadSound(SoundItemRocket);
	Tag = Object::tag::Item;
	sprite = _sprite;
	AnimationItem = new Animation(info_item);
	this->Samus = Samus;
	AllowDraw = true;
	transform = D3DXVECTOR2(0, 0);
}
Items::~Items()
{
	delete sounditemHP;
	delete sounditemrocket;

	delete AnimationItem;
}

//Tạo Items mới
void Items::NewItems(D3DXVECTOR2 pos, Itemtype type )
{
	AllowDraw = true;
	position = pos;
	ItemType = type;

	switch (ItemType)
	{
	case Items::HpItem:
		GetBound(4, 4);
		break;
	case Items::RocketItem:
		GetBound(4, 8);
		break;
	default:
		GetBound(14, 14);
		break;
	}
}

void Items::ChangeAnimation(Keyboard* key)
{
	switch (ItemType)
	{
	case Items::HpItem:
		AnimationItem->SetFrame(position, false, ItemDelay, 28, 29);
		break;
	case Items::RocketItem:
		AnimationItem->SetFrame(position, false, ItemDelay, 30, 31);
		break;
	case Items::Marimaru:
		AnimationItem->SetFrame(position, false, ItemDelay, 8, 11);
		break;
	case Items::Rocket:
		AnimationItem->SetFrame(position, false, ItemDelay, 4, 7);
		break;
	case Items::Icebean:
		AnimationItem->SetFrame(position, false, ItemDelay, 16, 19);
		break;
	case Items::Boom:
		AnimationItem->SetFrame(position, false, ItemDelay, 12, 15);
		break;
	case Items::Longbean:
		AnimationItem->SetFrame(position, false, ItemDelay, 20, 23);
		break;
	case Items::Tank:
		AnimationItem->SetFrame(position, false, ItemDelay, 0, 3);
		break;
	case Items::Varia:
		AnimationItem->SetFrame(position, false, ItemDelay, 24, 27);
		break;
	default:
		break;
	}
}


void Items::Update(float gameTime, Keyboard* key)
{
	ChangeAnimation(key);
	AnimationItem->Update(gameTime, key);
}

void Items::Render(Viewport *view)
{
	//Vẽ items
	if (AllowDraw)
	{
		if (view->isContains(bound))
		{
			sprite->SetData(
				AnimationItem->GetRect(),
				AnimationItem->GetCenter(),
				AnimationItem->GetPosition(),
				AnimationItem->GetScale(),
				AnimationItem->GetTransform(),
				AnimationItem->GetAngle()
			);
			sprite->Render(view);
		}
	}
}
Items::Itemtype Items::GetType(string name)
{
	if (name == "HpItem")
		return HpItem;
	if (name == "RocketItem")
		return RocketItem;
	if (name == "MaruMari")
		return Marimaru;
	if (name == "MissileRocket")
		return Rocket;
	if (name == "IceBean")
		return Icebean;
	if (name == "Boom")
		return Boom;
	if (name == "LongBean")
		return Longbean;
	if (name == "EnergyTank")
		return Tank;
	if (name == "Varia")
		return Varia;
}

//Va chạm Samus
void Items::OnCollision()
{
	AllowDraw = false;
	bound = { 0, 0, 0, 0 };
	switch (ItemType)
	{
	case Items::HpItem:
	{
		sound->PlaySoundA(sounditemHP);
		if (Samus->GetHP() < Samus->Tank * 100)
		{
			Samus->SetHP(Samus->GetHP() + 5);
			if (Samus->GetHP() >= Samus->Tank * 100)
				Samus->SetHP(Samus->Tank * 100 - 1);
		}
		break;
	}
	case Items::RocketItem:
	{
		//Nếu đạn rocket hiện tại lớn hơn số rocket tối đa thì bằng
		sound->PlaySoundA(sounditemrocket);
		Samus->rocketbullet += 2;
		if (Samus->rocketbullet > Samus->MaxRocket)
			Samus->rocketbullet = Samus->MaxRocket;
		break;
	}
	case Items::Marimaru:
	{
		Samus->FlagItems.maru = true;
		break;
	}
	case Items::Rocket:
	{
		//Tăng đạn và số lượng chứa lên 5
		Samus->rocketbullet += 5;
		Samus->MaxRocket += 5;
		Samus->FlagItems.rocket = true;
		break;
	}
	case Items::Icebean:
	{
		
		for (int i = 0; i < BulletMax; i++)
			Samus->bullet[i]->SetBulletType(Bullet::IceBullet);
		Samus->FlagItems.icebean = true;
		break;
	}
	case Items::Boom:
	{
		Samus->FlagItems.boom = true;
		break;
	}
	case Items::Longbean:
	{
		Samus->SBullet = std::numeric_limits<float>::infinity();
		break;
	}
	case Items::Tank:
	{
		Samus->Tank += 1;
		Samus->SetHP(Samus->Tank * 100 - 1);
		break;
	}
	case Items::Varia:
	{
		Samus->pathPNG = SamusAranVariaPNG;
		Samus->FlagItems.varia = true;
		Samus->changevaria = true;
		Samus->ChangeSkin(SamusAranVariaPNG);
	}
	break;
	default:
		break;
	}
}