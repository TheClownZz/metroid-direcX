#include "SamusAran.h"


SamusAran::SamusAran(Sprite* sprite_samus, Sprite* sprite_bullet, InfoSprite* info_bullet, Sound *sound)
{
	countdie = 1;
	posxdie = 16.0f;
	posydie = 16.0f;
	this->sound = sound;
	Damage = -1;
	soundrun = sound->LoadSound(SoundRun);
	soundjump = sound->LoadSound(SoundJump);
	soundSamusCollision = sound->LoadSound(SoundSamusCollision);
	sounddying = sound->LoadSound(SoundDying);
	sounddie = sound->LoadSound(SoundDie);

	Tag = Object::player;
	pathPNG = SamusAranNormalPNG;
	sprite = sprite_samus;
	//Tạo Animation
	info_samus = new InfoSprite(SamusAranXML);
	AnimationSamus = new SamusAranAnimation(info_samus);
	//Tạo StateSamus lấy state
	StateSamus = new SamusAranState(this);
	//Tạo va chạm
	CollisionSamus = new SamusAranCollision(this);
	//Tạo hộp đạn
	for (int i = 0; i < 8; i++)
	{
		bullet[i] = new Bullet(sprite_bullet,info_bullet, sound);
	}
	//Chuyển sang boom
	for (int i = 5; i < 8; i++)
		bullet[i]->SetBulletType(Bullet::Boom);

	//Chỉ số liên quan đến Object Bullet
	SBullet = SBulletMax;
	bulletorder = 0;
	boomorder = 5;
	rocketbullet = 100;
	Tank = 1;
	MaxRocket = 100;
	HP = 30;
	checkupshoot = false;
	changeGun = true;
	RocketGun = false;
	changevaria = false;

	position = D3DXVECTOR2(0, 0);
	velocity = D3DXVECTOR2(0, Gravity);
}

SamusAran::~SamusAran()
{
	delete soundrun;
	delete soundjump;
	delete soundSamusCollision;
	delete sounddying;
	delete sounddie;

	delete StateSamus;
	delete CollisionSamus;
	for (int i = 0; i < 8; i++)
	{
		Bullet* _bullet = bullet[i];
		delete _bullet;
	}
}

void SamusAran::New()
{
	AllowDraw = true;
	RocketGun = false;
	position = positionStart;
	//Chuyển trang phục
	ChangeSkin(pathPNG);
	//Chuyển đạn
	if (FlagItems.icebean)
		for (int i = 0; i < BulletMax; i++)
			bullet[i]->SetBulletType(Bullet::IceBullet);
	else
		for (int i = 0; i < BulletMax; i++)
			bullet[i]->SetBulletType(Bullet::NormalBullet);

	StateSamus->SetState(SamusAranState::Appearing);
	HP = 30;
	countdie = 1;
	posxdie = 16.0f;
	posydie = 16.0f;
}

//Lấy Board để xét va chạm
RECT SamusAran::GetBoard(D3DXVECTOR2 distance)
{
	return CollisionSamus->GetBoard(CollisionSamus->GetBound(), distance);
}

//Khóa chuyển động
void SamusAran::LockAnimation(bool Lock, int index)
{
	if (Lock)
	{
		SetVelocity(0, 0);
		if (index < 0)
		{
			AnimationSamus->SetPause(true, AnimationSamus->GetIndex());
			lock = true;
		}
		else 
			AnimationSamus->SetPause(true, index);
	}
	else
	 {
		lock = false;
		AnimationSamus->SetPause(false);
	}
}

//Đổi trang phục
void SamusAran::ChangeGun(Keyboard* key, bool Fire)
{
	if (Fire && changeGun)
	{
		changeGun = false;
		//Đang còn vẽ đạn không chuyển Súng
		for (int i = 0; i < BulletMax; i++)
			if (bullet[i]->GetAllowDraw())
				return;
		//Nếu đang là Rocket thì chuyển về bình thường
		if (RocketGun || !FlagItems.rocket)
		{
			RocketGun = false;
			//Chuyển trang phục
				ChangeSkin(pathPNG);
				changevaria = FlagItems.varia;
			//Chuyển đạn
			if (FlagItems.icebean)
				for (int i=0; i < BulletMax; i++)
					bullet[i]->SetBulletType(Bullet::IceBullet);
			else
				for (int i = 0; i < BulletMax; i++)
					bullet[i]->SetBulletType(Bullet::NormalBullet);
		}
		else
		{
			RocketGun = true;
			changevaria = false;
			ChangeSkin(SamusAranRocketPNG);
			for (int i = 0; i < BulletMax; i++)
				bullet[i]->SetBulletType(Bullet::RocketBullet);
		}
	}
}

//Đổi trạng thái
void SamusAran::ChangeAnimation(float gameTime, Keyboard* key)
{
	if (HP < 15)
	{
		sound->PlaySoundA(sounddying);
	}

	if (!lock)
	{
		//Lấy trạng thái Animation
		checkupshoot = false;
		StateSamus->MoveX(key);
		//Đổi trạng thái Samus
		switch (StateSamus->GetState())
		{
			//xuất hiện
		case SamusAranState::Appearing:
		{
			StateSamus->AppearState(key, AnimationSamus->GetIndex());
			AnimationSamus->Appear();
			break;
		}
		//đứng
		case SamusAranState::Standing:
		{
			StateSamus->StandState(key, FlagItems.maru);
			AnimationSamus->Stand();
			break;
		}
		//chạy
		case SamusAranState::Running:
		{
			sound->PlaySoundA(soundrun);
			
			StateSamus->RunState(key);
			AnimationSamus->Run();
			break;
		}
		//nhảy
		case SamusAranState::Jumping:
		{
			if (StateSamus->GetJump())
				sound->PlaySoundA(soundjump);
			StateSamus->JumpState(key);
			AnimationSamus->Jump();
			break;
		}
		//lăn
		case SamusAranState::Rolling:
		{
			StateSamus->RollState(key, FlagItems.boom);
			AnimationSamus->Roll();
			break;
		}
		//đứng giơ súng lên
		case SamusAranState::Standingup:
		{
			StateSamus->StandUpState(key);
			AnimationSamus->StandUp();
			break;
		}
		//chạy giơ súng lên
		case SamusAranState::Runningup:
		{
			sound->PlaySoundA(soundrun);

			StateSamus->RunUpState(key);
			AnimationSamus->RunUp();
			break;
		}
		//nhảy giơ súng lên
		case SamusAranState::Jumpingup:
		{
			if (StateSamus->GetJump())
				sound->PlaySoundA(soundjump);
			StateSamus->JumpUpState(key);
			AnimationSamus->JumpUp();
			break;
		}
		//Chạy và nhảy
		case SamusAranState::Runningjump:
		{
			if (StateSamus->GetJump())
				sound->PlaySoundA(soundjump);

			StateSamus->RunJumpState(key);

			if (StateSamus->GetJumpHeight() <= 16)
				AnimationSamus->Jump();
			else
				AnimationSamus->RunJump();

			break;
		}
		//Đứng bắn
		case SamusAranState::standShoot:
		{
			StateSamus->ShootState(key);
			AnimationSamus->StandShoot();
			break;
		}
		//Chạy bắn
		case SamusAranState::runShoot:
		{
			sound->PlaySoundA(soundrun);

			StateSamus->ShootState(key);
			AnimationSamus->RunShoot();
			break;
		}
		//Nhảy bắn
		case SamusAranState::jumpShoot:
		{
			StateSamus->ShootState(key);
			AnimationSamus->JumpShoot();
			break;
		}
		//Thả boom
		case SamusAranState::rollShoot:
		{
			StateSamus->ShootState(key);
			break;
		}
		//Đứng bắn lên
		case SamusAranState::standupShoot:
		{
			checkupshoot = true;
			StateSamus->ShootState(key);
			AnimationSamus->StandUpShoot();
			break;
		}
		//chạy bắn lên
		case SamusAranState::runupShoot:
		{
			sound->PlaySoundA(soundrun);

			checkupshoot = true;
			StateSamus->ShootState(key);
			AnimationSamus->RunUp();
			break;
		}
		//nhảy bắn lên
		case SamusAranState::jumpupShoot:
		{
			checkupshoot = true;
			StateSamus->ShootState(key);
			AnimationSamus->JumpUpShoot();
			break;
		}
		//chết
		case SamusAranState::Dying:
		{
			sound->PlaySoundA(sounddie);
			StateSamus->DeadState();
			AnimationSamus->Die();
			break;
		}
		break;
		default:
			break;
		}

		StateName = StateSamus->GetState();
	}
	//Update State
	StateSamus->Update(gameTime, key);

	//Kiểm tra trạng thái tiếp theo
	CollisionSamus->SetNextState(StateSamus->GetState());
	//SetBound
	CollisionSamus->SetBound(StateSamus->GetState());
	//Va chạm Boom và Samus
	for (int i = BulletMax; i < 8; i++)
	{
		if (bullet[i]->GetStateBullet() == Bullet::Bursting)
		{
			if (Collision::isCollision(bullet[i]->GetBound(), bound))
			{
				int side;
				if (position.x < bullet[i]->GetPosition().x)
					side = 1;
				else if (position.x > bullet[i]->GetPosition().x)
					side = -1;
				else
					side = 0;
				StateSamus->BleedState(side, -1, false);
			}
		}
	}
}

//Mất máu
void SamusAran::SetBleedState(int side, int dame, bool Bleed)
{
	StateSamus->BleedState(side, dame, Bleed);
}

//Kiểm tra va chạm 
void SamusAran::OnCollision(Object *other, D3DXVECTOR2 distance, D3DXVECTOR2 disSamus)
{
	{
		RECT board = this->GetBoard(distance);
		//Nếu other trong vùng di chuyển
		if (CollisionSamus->isCollision(board, other->GetBound()))
		{
			if (!CollisionSamus->isCollision1(GetBound(), other->GetBound()))
			{
				//Lấy cạnh va chạm
				D3DXVECTOR2 SideCollision;
				//lấy thời gian va chạm
				float Time = CollisionSamus->CollisionAABB(GetBound(), other->GetBound(), distance, SideCollision);
				
				//bé hơn 1 thì có va chạm
				if (Time < 1.0f)
				{
					//Chạm trục nào update rồi cho vận tốc bằng không
					if (other->Tag == Object::Enemy)
					{
						if (changevaria && StateName == SamusAranState::Runningjump)
						{
							other->OnCollision(this);
							return;
						}
					}

					if (SideCollision.x != 0.0f)
					{
						position.x += disSamus.x*Time;
						velocity.x = 0;

						switch (StateName)
						{
							//Va chạm khi chạy
						case SamusAranState::Running:
						case SamusAranState::Runningup:
						case SamusAranState::runShoot:
						case SamusAranState::runupShoot:
						{
							CollisionSamus->RunCollision();
							break;
						}
						case SamusAranState::Runningjump:
						default:
							break;
						}
					}
					else if (SideCollision.y == -1.0f)
					{
						position.y += disSamus.y*Time;
						StateSamus->SetFall(true);
						velocity.y = -Gravity;
					}
					else if (SideCollision.y == 1.0f)
					{
						position.y += disSamus.y*Time;
						StateSamus->SetFall(true);
						//Rolling thì cho ngược vận tốc để nảy
						if (StateName == SamusAranState::Rolling || StateName == SamusAranState::rollShoot)
							velocity.y = -velocity.y / 1.5f;
						else
							velocity.y = 0;
					}
					//Chuyển trạng thái sau khi va chạm
					StateSamus->SetState(CollisionSamus->GetNextState());

					//Enemy
					if (other->Tag == Object::Enemy)
					{
						int side;
						if (position.x < other->GetPosition().x)
							side = 1;
						else
							side = -1;
						if (other->GetDamage() > 0)
						{
							sound->PlaySoundA(soundSamusCollision);
							StateSamus->BleedState(side, other->GetDamage(), true);
						}
					}
					//
				}
			}
			else
			{
				if (other->Tag != Object::Enemy)
				{
					CollisionSamus->CollisionFalse(other->GetBound());
					StateSamus->SetState(CollisionSamus->GetNextState());
				}
			}
		}
	}
}

void SamusAran::Update(float gameTime, Keyboard* key)
{
	//Lấy trạng thái trước để xét va chạm
	CollisionSamus->SetStateBefore(StateSamus->GetState());

	//Chuyển súng
	if (key->GIsKeyUp(Dik_SELECT))
	{
		changeGun = true;
	}
	if (key->IsKeyDown(Dik_SELECT))
		ChangeGun(key, true);

	//Chỉnh lại vị trí sau khi xét va chạm
	Object::Update(gameTime, key);
	//Update Animation
	
	AnimationSamus->SetPosition(D3DXVECTOR2( position.x , position.y - Height / 2));
	AnimationSamus->SetPositionRunJump();

	AnimationSamus->SetFlipFlag(GetFlipFlag());
	AnimationSamus->Update(gameTime, key);
	//Khi trạng thái lăn thì nó nảy
	AnimationSamus->SetTransRoll(StateSamus->GetState(), 0.1);

	//Lấy vị trí và tạo viên đạn
	if (StateSamus->GetShoot())
	{
		if (StateName == SamusAranState::Rolling)
		{
			if (!bullet[boomorder]->GetAllowDraw())
			{
				bullet[boomorder]->NewBullet(position + AnimationSamus->GetLocalGun(AnimationSamus->GetIndex()), FlipFlag, false, 1);
				//Chuyển qua vị trí đạn tiếp the	
				boomorder++;
				if (boomorder > 7)
					boomorder = BulletMax;
			}
		}
		else
		{
			if (!bullet[bulletorder]->GetAllowDraw())
			{
				if (bullet[bulletorder]->GetBulletType() != Bullet::RocketBullet)
				{
					float TimeFire = SBullet / (BulletSpeed*100.0f);
					bullet[bulletorder]->NewBullet(position + AnimationSamus->GetLocalGun(AnimationSamus->GetIndex()), FlipFlag, checkupshoot, TimeFire);
					//Chuyển qua vị trí đạn tiếp theo
					bulletorder++;
					if (bulletorder >= BulletMax)
						bulletorder = 0;
				}
				else
				{
					if (rocketbullet > 0)
					{
						rocketbullet--;
						float TimeFire = std::numeric_limits<float>::infinity();
						bullet[bulletorder]->NewBullet(position + AnimationSamus->GetLocalGun(AnimationSamus->GetIndex()), FlipFlag, checkupshoot, TimeFire);
						//Chuyển qua vị trí đạn tiếp theo
						bulletorder++;
						if (bulletorder >= BulletMax)
							bulletorder = 0;
					}
					else
					{
						ChangeGun(key, true);
					}
				}
			}
		}
	}
	//Update đạn cho vẽ thì mới update
	for (int i = 0; i < 8; i++)
	{
		if (bullet[i]->GetAllowDraw())
			bullet[i]->Update(gameTime, key);
	}

	if (key->IsKeyDown(DIK_G))
		LockAnimation(false, 01);
}

void SamusAran::Render(Viewport* viewport)
{
	//Vẽ đạn đã bắn
	for (int i = 0; i < 8; i++)
	{
		bullet[i]->Render(viewport);
	}

	//Vẽ Samus
	if (AllowDraw)
	{
		sprite->SetData(
			AnimationSamus->GetRect(),
			AnimationSamus->GetCenter(),
			AnimationSamus->GetPosition(),
			AnimationSamus->GetScale(),
			AnimationSamus->GetTransform(),
			AnimationSamus->GetAngle()
		);
		if (StateName != SamusAranState::Dying)
		{
			sprite->Render(viewport);
			return;
		}
		else
		{
			D3DXVECTOR2 posdie = position;
			posdie.y += 16;
			switch (countdie)
			{
			case 1:
			{
				posdie.x -= (posxdie/2);
				countdie = 2;
				break;
			}
			case 2:
			{
				posdie.x -= posxdie;
				posdie.y += posydie;
				countdie = 3;
				break;
			}
			case 3:
			{
				posdie.x += posxdie;
				posdie.y += posydie;
				countdie = 4;
				break;
			}
			case 4:
			{
				posdie.x += (posxdie / 2);
				countdie = 1;

				if (position.y > viewport->GetScene().bottom)
				{
					position.y -= 5;
					posxdie += 2;
					posydie -= 2;
				}
				else
					AllowDraw = false;
				break;
			}
			default:
				break;
			}

			sprite->SetPosition(posdie);
			sprite->Render(viewport);
		}
	}
	
}
