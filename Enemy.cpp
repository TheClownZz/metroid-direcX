
#include "Enemy.h"



Enemy::Enemy()
{
}

Enemy::Enemy(SamusAran *samus, Sprite* sprite_enemy, Sprite* sprite_item, Sprite* sprite_bullet, InfoSprite* info_bullet, InfoSprite* info_enemy, InfoSprite* info_item, Sound* sound)
{
	this->sound = sound;
	Tag = tag::Enemy;
	sprite = sprite_enemy;
	sprite_Bullet = sprite_bullet;
	this->info_bullet = info_bullet;
	this->Samus = samus;
	Item = new Items(Samus, sprite_item, info_item, sound);
	AnimationEnemy = new Animation(info_enemy);
	delay = 0;
	delayMove = 0;
	timedie = 0.0f;
	level = rand() % 3 + 0;
}

Enemy::~Enemy()
{
	delete AnimationEnemy;
	delete Item;
}

Enemy::Enemytype Enemy::GetEnemyType()
{
	return Enemytype();
}

void Enemy::SetEnemyType()
{
	GetBound(14, 14);
	Damage = 8;
	delayMove = 0;
	switch (EnemyType)
	{
	case Enemy::Zoomer:
	{
		frameEnemy = 8;
		if (position.x < Samus->GetPosition().x)
			velocity.x = -0.5f;
		else
			velocity.x = 0.5f;

		velocity.y = -0.5f;
		side.x = 0;
		side.y = 1;
		change = false;
		HP = 3 * (level + 1);
		break;
	}
	case Enemy::Skree:
	{
		KillYourSeft = false;
		delay = 40;
		frameEnemy = 3;
		HP = 3 * (level + 1);
		limit = 60;
		break;
	}
	case Enemy::Ripper:
	{
		frameEnemy = 1;
		HP = 1000 * (level + 1);
		SetVelocityX(-0.5f);
		isSeePlayer = false;
		break;
	}
	case Enemy::Rio:
	{
		frameEnemy = 2;
		HP = 10 * (level + 1);
		delay = 40;
		break;
	}
	case Enemy::Waver:
	{
		frameEnemy = 3;
		//Chỉ có 2 level
		if (level > 1)
			level = 1;

		HP = 8 * (level + 1);
		FlipFlag = false;
		SetVelocity(-0.75f, 1.0f);
		delay = 0;
		break;
	}
	case Enemy::Zeb:
	{
		frameEnemy = 2;
		HP = level + 1;
		GetBound(14, 14);
		limit = 128;
		break;
	}
	default:
		break;
	}
	//Lấy vận tốc 
	velocityold = velocity;
}

Enemy::Stateenemy Enemy::GetStateEnemy()
{
	return Stateenemy();
}

void Enemy::SetStateEnemy(Stateenemy stateenemy)
{
	StateEnemy = stateenemy;
}

void Enemy::NewEnemy(D3DXVECTOR2 pos, Enemytype type)
{
	Item->NewItems(positionStart, Items::None);
	Item->OnCollision();
	AllowDraw = true;
	StateEnemy = Moving;
	velocity = D3DXVECTOR2(0, 0);
	positionStart = pos;
	position = pos;
	EnemyType = type;
	SetEnemyType();
}

Enemy::Enemytype Enemy::GetType(string name)
{
	if (name == "Zoomer")
		return Zoomer;
	if (name == "Skree")
	{
		for (int i = 0; i < 4; i++)
		{
			bullet_skree[i] = new Bullet(sprite_Bullet, info_bullet, sound);
		}
		return Skree;
	}
	if (name == "Ripper")
		return Ripper;
	if (name == "Rio")
		return Rio;
	if (name == "Waver")
		return Waver;
	if (name == "Zeb")
		return Zeb;
}
void Enemy::ChangeAnimation(Keyboard * key)
{
	switch (EnemyType)
	{
	case Enemy::Rio:
	{
		GetBound(14, 14);
		switch (StateEnemy)
		{
		case Enemy::Moving:
			AnimationEnemy->SetFrame(position, FlipFlag, 2, 49 + level * 2, 50 + level * 2);
			break;
		case Enemy::Dying:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 78, 80);
			break;
		}
		break;
	}
	case Enemy::Zoomer:
	{
		GetBound(12, 12);
		switch (StateEnemy)
		{
		case Enemy::Moving:
			AnimationEnemy->SetFrame(position, FlipFlag, 2, 8 + level * 8, 9 + level * 8);
			break;
		case Enemy::ZoomerMoveLeft:
			AnimationEnemy->SetFrame(position, FlipFlag, 2, 10 + level * 8, 11 + level * 8);
			break;
		case Enemy::ZoomerMoveTop:
			AnimationEnemy->SetFrame(position, FlipFlag, 2, 12 + level * 8, 13 + level * 8);
			break;
		case Enemy::ZoomerMoveRight:
			AnimationEnemy->SetFrame(position, FlipFlag, 2, 14 + level * 8, 15 + level * 8);
			break;
		case Enemy::Dying:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 78, 80);
			break;
		}
		break;
	}
	case Enemy::Skree:
	{
		GetBound(14, 28);
		switch (StateEnemy)
		{
		case Enemy::Moving:
			AnimationEnemy->SetFrame(position, FlipFlag, 4, 35 + level * 3, 37 + level * 3);
			break;
		case Enemy::Dying:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 78, 80);
			break;
		}
		break;
	}
	case Enemy::Ripper:
	{
		GetBound(14, 8);
		switch (StateEnemy)
		{
		case Enemy::Moving:
			AnimationEnemy->SetFrame(position, FlipFlag, 0, 56 + level, 56 + level);
			break;
		case Enemy::Dying:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 78, 80);
			break;
		}
		break;
	}
	case Enemy::Waver:
	{
		GetBound(14, 14);
		switch (StateEnemy)
		{
		case Enemy::Moving:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 63 + level * 3, 65 + level * 3);
			break;
		case Enemy::Dying:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 78, 80);
			break;
		}
		break;
	}
	case Enemy::Zeb:
	{
		GetBound(14, 14);
		switch (StateEnemy)
		{
		case Enemy::Moving:
			AnimationEnemy->SetFrame(position, FlipFlag, 2, 71 + level * 2, 72 + level * 2);
			break;
		case Enemy::Dying:
			AnimationEnemy->SetFrame(position, FlipFlag, 3, 78, 80);
			break;
		}
		break;
	}

	}
}

void Enemy::Update(float gameTime, Keyboard * key)
{
	Object::Update(gameTime, key);
	ChangeAnimation(key);
	AnimationEnemy->Update(gameTime, key);

	if (StateEnemy == Dying)
	{
		if (EnemyType == Skree)
		{
			if (KillYourSeft == true)
			{
				delay--;
				if (delay == 0)
				{
					StateEnemy = Dying;
					HP = 0;
					for (int i = 0; i < 4; i++)
					{
						float timefire = 48 / (BulletSpeed*100.0f);
						bullet_skree[i]->NewBullet(this->position, false, false, timefire);
					}
					bullet_skree[0]->SetVelocity(BulletSpeed, 0);
					bullet_skree[1]->SetVelocity(BulletSpeed, BulletSpeed / 1.5f);
					bullet_skree[2]->SetVelocity(-BulletSpeed, BulletSpeed / 1.5f);
					bullet_skree[3]->SetVelocity(-BulletSpeed, 0);
				}
				else if (delay < 0)
					for (int i = 0; i < 4; i++)
					{
						bullet_skree[i]->Update(gameTime, key);
						if (Collision::isCollision(bullet_skree[i]->GetBound(), Samus->GetBound()))
						{
							int side;
							if (position.x > Samus->GetPosition().x)
								side = 1;
							else
								side = -1;
							Samus->SetBleedState(side, Damage, true);
							bullet_skree[i]->OnCollision();
						}
					}
			}
		}

		timedie += gameTime;
		SetVelocity(0, 0);
		if (timedie > 0.3f)
		{
			timedie = 0.0f;
			AllowDraw = false;
		}
		if (Collision::isCollision(Samus->GetBound(), Item->GetBound()))
		{
			Item->OnCollision();
		}
		Item->Update(gameTime, key);
		if (EnemyType == Zeb && !Item->GetAllowDraw() || timedie > 5.0f)
		{
			NewEnemy(positionStart, EnemyType);
		}
	}
}

void Enemy::Render(Viewport * view)
{
	if (view->isContains(bound))
	{
		if (AllowDraw)
		{
			sprite->SetData(
				AnimationEnemy->GetRect(),
				AnimationEnemy->GetCenter(),
				AnimationEnemy->GetPosition(),
				AnimationEnemy->GetScale(),
				AnimationEnemy->GetTransform(),
				AnimationEnemy->GetAngle()
			);
			sprite->Render(view);
		}

		if (StateEnemy == Dying)
		{
			Item->Render(view);

			if (EnemyType == Skree)
				for (int i = 0; i < 4; i++)
					if (bullet_skree[i]->GetAllowDraw())
						bullet_skree[i]->Render(view);
		}
	}
	else
	{
		if (EnemyType == Zeb)
			NewEnemy(positionStart, EnemyType);

		if (!Collision::isCollision(positionStart.x, positionStart.y, view->GetBoundViewport()))
		{
			NewEnemy(positionStart, EnemyType);
		}
	}
}

void  Enemy::OnCollision(Object *obj, D3DXVECTOR2 distance)
{
	RECT board = Collision::GetBoard(bound, distance);
	//Nếu other trong vùng di chuyển
	if (Collision::isCollision(board, obj->GetBound()))
	{
		//lấy thời gian va chạm
		//Lấy cạnh va chạm
		D3DXVECTOR2 SideCollision;
		float Time = Collision::CollisionAABB(bound, obj->GetBound(), distance, SideCollision);
		
		//bé hơn 1 thì có va chạm
		if (Time < 1.0f)
		{
			if (SideCollision.x != 0)
				position.x += distance.x*Time;
			else if (SideCollision.y != 0)
				position.y += distance.y*Time;

			switch (EnemyType)
			{
			//Zoomer
			case Enemy::Zoomer:
			{
				if (SideCollision.x != 0)
				{
					side.x = 1;
					velocity.x = 0;
				}
				else if (SideCollision.y != 0)
				{
					side.y = 1;
					velocity.y = 0;
				}
				break;
			}
			//Skree
			case Enemy::Skree:
			{
				if (SideCollision.y == 1)
				{
					StateEnemy = Dying;
					HP = 0;
					KillYourSeft = true;
					SetVelocity(0, 0);
				}
				break;
			}
			//Ripper
			case Enemy::Ripper:
			{
				FlipFlag = !this->FlipFlag;
				break;
			}
			//Rio
			case Enemy::Rio:
			{
				if (SideCollision.y == -1)
				{
					SetVelocity(0, 0);
				}
				else if (SideCollision.y == 1)
				{
					velocity.y = 0.0f;
				}
				else if (SideCollision.x != 0)
				{
					SetVelocity(-GetVelocity().x, 0.1);
				}
				break;
			}
			//Waver
			case Enemy::Waver:
			{
				if (SideCollision.x != 0)
				{
					FlipFlag = !this->FlipFlag;
					velocity.x = -velocity.x;
				}
				else if (SideCollision.y != 0)
				{
					velocity.y = 0;
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

void Enemy::OnCollision()
{
	//delayMove dùng để dóng băng hay không cho Enemy di chuyển
	delayMove--;
	if (delayMove <= 0 && StateEnemy != Dying)
	{
		Damage = 8;
		switch (EnemyType)
		{
			//Zoomer
		case Enemy::Zoomer:
		{
			velocity = velocityold;

			if (side.x == 0 && side.y == 1)
			{
				change = true;
				if (velocity.y > 0)
					StateEnemy = ZoomerMoveTop;
				else
					StateEnemy = Moving;
			}
			else if (side.x == 1 && side.y == 0)
			{
				change = true;
				if (velocity.x > 0)
					StateEnemy = ZoomerMoveRight;
				else
					StateEnemy = ZoomerMoveLeft;
			}
			else if (side.x == 1 && side.y == 1 && change)
			{
				change = false;
				if (StateEnemy == Moving || StateEnemy == ZoomerMoveTop)
					velocity.y = -velocity.y;
				else
					velocity.x = -velocity.x;
			}
			else if (side.x == 0 && side.y == 0 && change)
			{
				change = false;
				if (StateEnemy == Moving || StateEnemy == ZoomerMoveTop)
					velocity.x = -velocity.x;
				else
					velocity.y = -velocity.y;
			}

			side.x = 0;
			side.y = 0;
			velocityold = velocity;

			break;
		}

		//Skree
		case Enemy::Skree:
		{
			StateEnemy = Moving;
			if (!(Samus->GetBound().right < position.x - limit || Samus->GetBound().left > position.x + limit))
			{
				if (Samus->GetPosition().x < GetPosition().x)
					velocity.x = -0.25;
				else
					velocity.x = 0.25;
				velocity.y = -Gravity;
			}

			break;
		}

		//Ripper
		case Enemy::Ripper:
		{
			StateEnemy = Moving;
			if (FlipFlag)
				velocity.x = 0.5f;
			else
				velocity.x = -0.5;

			break;
		}
		case Enemy::Rio:
		{
			StateEnemy = Moving;
			// xac dinh huong cua player khi dau
			if (velocity.x == 0)
			{
				if (delay <= 0)
					delay = 40;
				delay--;
				if (delay <= 0)
				{
					if (Samus->GetPosition().x < position.x)
						velocity.x = -0.75;
					else
						velocity.x = 0.75;

					if (velocity.y == 0)
						velocity.y = -4.05;
				}

			}
			if (!(bound.bottom > Samus->GetBound().top || bound.top < Samus->GetBound().bottom)
				&& velocity.y > -0.25)
				isSeePlayer = true;
			else
				isSeePlayer = false;

			if (Collision::isCollision(bound, Samus->GetBound()))
				isSeePlayer = false;

			if (!isSeePlayer)
			{
				if (!(velocity.x == 0 && velocity.y == 0))
					AddVelocity(0, 0.1);
			}

			break;
		}
		case Enemy::Waver:
		{
			StateEnemy = Moving;
			if (delayMove == 0)
				velocity = velocityold;

			if (delay >= 180)
				delay = 0;

			if (delay == 0)
				velocity.y = RunSpeed;
			else if (delay == 60 || delay == 150)
				velocity.y = 0;
			else if (delay == 90)
				velocity.y = -RunSpeed;
			delay++;

			velocityold = velocity;
			break;
		}
		case Enemy::Zeb:
		{
			StateEnemy = Moving;
			if (!(Samus->GetBound().right < position.x - limit || Samus->GetBound().left > position.x + limit))
			{
				if (velocity.x == 0)
				{
					if (position.y >= Samus->GetPosition().y && position.y >= positionStart.y + 24)
					{
						if (position.x < Samus->GetPosition().x)
						{
							FlipFlag = true;
							SetVelocity(1.5f, 0);
						}
						else
						{
							FlipFlag = false;
							SetVelocity(-1.5f, 0);
						}
					}
					else
						velocity.y = 0.75f;
				}
			}
			break;
		}
		default:
			break;
		}
	}
	else
	{
		//Nháy để chuẩn bị từ Frezz về Move
		if (delayMove > 30 && delayMove <= 80)
		{
			if (delayMove % 10 == 0)
			{
				AnimationEnemy->SetFrame(position, FlipFlag, 0, indexFrezz, indexFrezz);
			}
			else if (delayMove % 5 == 0)
			{
				int idex = indexFrezz + (level + 1) * frameEnemy;
				AnimationEnemy->SetFrame(position, FlipFlag, 0, idex, idex);
			}
		}
	}
}

void Enemy::OnCollision(Object *obj)
{
	if (EnemyType != Ripper)
	{
		delayMove = 30;
		SetVelocity(0.0f, 0.0f);
		HP -= obj->GetDamage();

		//Đóng băng và rã băng Enemy
		if (obj->GetDamage() == 2 && StateEnemy != Frezing)
		{
			delayMove = 500;
			Damage = 0;
			indexFrezz = AnimationEnemy->GetIndex() - (level + 1) * frameEnemy;
			AnimationEnemy->SetFrame(position, FlipFlag, 0, indexFrezz, indexFrezz);
			StateEnemy = Frezing;
		}
		else
			StateEnemy = Moving;

		//Kiểm tra HP
		if (HP <= 0 || obj->GetDamage() == 6 || obj->GetDamage() == -1)
		{
			HP = 0;
			StateEnemy = Dying;

			int item = rand() % 4 + 0;
			switch (item)
			{
			case 2:
				Item->NewItems(position, Items::HpItem);
				break;
			case 3:
				if (Samus->FlagItems.rocket)
					Item->NewItems(position, Items::RocketItem);
				break;
			default:
				break;
			}
		}

	}
	else
	{
		//Đóng băng Ripper
		if (obj->GetDamage() == 2)
		{
			delayMove = 500;
			if (StateEnemy != Frezing)
			{
				Damage = 0;
				indexFrezz = AnimationEnemy->GetIndex() - (level + 1) * frameEnemy;
				AnimationEnemy->SetFrame(position, FlipFlag, 0, indexFrezz, indexFrezz);
				StateEnemy = Frezing;
				SetVelocity(0.0f, 0.0f);
			}
		}
	}
}