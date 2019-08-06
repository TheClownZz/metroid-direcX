#include "SamusAranState.h"


SamusAranState::SamusAranState(Object* samus)
{
	this->Samus = samus;
	StateName = Appearing;
	jump = true;
	fall = false;
	timeShoot = 0.0f;
	timeBleed = 2.1f;
	timeapear = 0;
	StartJump = 0.0f;
}

SamusAranState::SamusAranState()
{
}

SamusAranState::~SamusAranState()
{
}

//lấy trạng thái bắn
bool SamusAranState::GetShoot()
{
	return shoot;
}

//lấy trạng thái nhảy
bool SamusAranState::GetJump()
{
	return jump;
}

//Lấy trạng thái
SamusAranState::Statename SamusAranState::GetState()
{
	return StateName;
}
float SamusAranState::GetJumpHeight()
{
	return Samus->GetPosition().y - StartJump;
}
//Set trạng thái
void SamusAranState::SetState(Statename state)
{
	StateName = state;
}
void SamusAranState::SetFall(bool fall_)
{
	fall = fall_;
}

//Kiểm tra di chuyển
void SamusAranState::MoveX(Keyboard* key)
{
	if (key->IsKeyDown(Dik_RIGHT))
	{
		Samus->SetVelocityX(RunSpeed);
		Samus->SetFlipFlag(false);
	}
	else if (key->IsKeyDown(Dik_LEFT))
	{
		Samus->SetVelocityX(-RunSpeed);
		Samus->SetFlipFlag(true);
	}
	else Samus->SetVelocityX(0.0f);
}

//Rơi
void SamusAranState::Fall(bool &fall)
{
	if (fall)
	{
		if (jump || (Samus->GetPosition().y - StartJump) >= JumpMin)
		{
			if (Samus->GetVelocity().y > -Gravity && Samus->GetVelocity().y <= 0.0f)
			{
				Samus->SetVelocityY(Samus->GetVelocity().y - FallAc);
			}
			else if (Samus->GetVelocity().y <= -Gravity || (Samus->GetPosition().y - StartJump) <= 2 * JumpMin)
			{
				Samus->SetVelocityY(-Gravity);
			}
			else
			{
				Samus->SetVelocityY(-FallAc * 2);
			}
		}
	}
	else
	{
		if ((jump && Samus->GetVelocity().y > 0.0f) || ((Samus->GetPosition().y - StartJump) >= 2 * JumpMin  && Samus->GetVelocity().y > 0.0f))
		{
			Samus->SetVelocityY(Samus->GetVelocity().y - FallAc);
		}
		else if (Samus->GetVelocity().y <= 0.0f)
			fall = true;
	}
}

//rơi có đàn hồi dành cho Roll
void SamusAranState::FallRoll(bool &fall)
{
	if (fall) //Trạng thái rơi riêng của Rolling
	{
		if (Samus->GetVelocity().y <= -Gravity*2.0f)
			Samus->SetVelocityY(-Gravity);
		else
			Samus->SetVelocityY(Samus->GetVelocity().y - FallAc);
	}
	else
	{
		if (Samus->GetVelocity().y <= 0.0f)
		{
			Samus->SetVelocityY(0.0f);
			fall = true;
		}
		else
		{
			Samus->SetVelocityY(Samus->GetVelocity().y - FallAc);
		}
	}
}

//Trạng thái bắt đầu
void SamusAranState::AppearState(Keyboard* key, int Index)
{
	jump = true;
	fall = false;
	shoot = false;
	Samus->SetVelocity(0.0f, 0.0f);
	Samus->SetFlipFlag(false);
	StateName = Appearing;/////

	if (Index == 3)
	{
		Samus->SetAllowDraw(true);
		Samus->LockAnimation(true, Index);

		if (timeapear < 3.25f)
			return;

		if (key->IsKeyDown(Dik_UP) || key->IsKeyDown(Dik_SHOOT) || key->IsKeyDown(Dik_LEFT) || key->IsKeyDown(Dik_RIGHT) || key->IsKeyDown(Dik_JUMP))
		{
			timeapear = 0.0f;
			Samus->LockAnimation(false, Index);
			Samus->SetPosition(D3DXVECTOR2(Samus->GetPosition().x, Samus->GetPosition().y + 3));
			StateName = Standing;
		}
	}
	else
	{
		if (appear)
		{
			Samus->SetAllowDraw(true);
			appear = false;
		}
		else
		{
			Samus->SetAllowDraw(false);
			appear = true;
		}
	}
}

//Trạng thái Đứng
void SamusAranState::StandState(Keyboard* key, bool isroll)
{
	jump = true;//Cho nhảy khi đứng hoặc chạy
	fall = false;//cho nhảy thì không rơi
	shoot = false;//chỉ bắn trong Shotting

	Samus->SetVelocityY(-Gravity);

	if (key->IsKeyDown(Dik_UP))
	{
		StateName = Standingup;
	}
	else if (key->IsKeyDown(Dik_JUMP))
	{
		StateName = Jumping;
	}
	else if (key->IsKeyDown(Dik_LEFT) || key->IsKeyDown(Dik_RIGHT))
	{
		StateName = Running;
	}
	else if (key->IsKeyDown(Dik_DOWN) && isroll)
	{
		fall = true;
		Samus->SetVelocityY(-FallAc);
		StateName = Rolling;
	}
	else if (key->IsKeyDown(Dik_SHOOT))
	{
		StateName = standShoot;
	}
	else
	{
		StateName = Standing;/////
	}
}

//Trang thái chạy
void SamusAranState::RunState(Keyboard* key)
{
	jump = true;
	fall = false;
	shoot = false;

	if (key->IsKeyDown(Dik_LEFT) || key->IsKeyDown(Dik_RIGHT))
	{
		if (key->IsKeyDown(Dik_UP))
		{
			StateName = Runningup;
		}
		else if (key->IsKeyDown(Dik_JUMP))
		{
			StateName = Runningjump;
		}
		else if (key->IsKeyDown(Dik_SHOOT))
		{
			StateName = runShoot;
		}
		else
		{
			fall = true;
			Fall(fall);
			StateName = Running;/////
			if (Samus->GetVelocity().y < -FallAc)
			{
				jump = false;
				Samus->SetVelocity(Samus->GetVelocity().x, -Gravity);
				StateName = Jumping;
			}
		}
	}
	else StateName = Standing;
}

//Trạng thái nhảy
void SamusAranState::JumpState(Keyboard* key)
{
	shoot = false;

	if (jump)
	{
		StartJump = Samus->GetPosition().y;
		Samus->SetVelocityY(JumpSpeed);
		if (!bleed)
		{
			push = false;
			Samus->SetAllowDraw(true);
			timeBleed = 2.1f;
		}
	}
	jump = false;

	if (key->GIsKeyUp(Dik_JUMP) || (Samus->GetPosition().y - StartJump) >= JumpMax)
		fall = true;

	if (fall && Samus->GetVelocity().y == 0.0f)
	{
		StateName = Standing;
	}
	else
	{
		if (key->IsKeyDown(Dik_UP))
		{
			StateName = Jumpingup;
		}
		else if (key->IsKeyDown(Dik_SHOOT))
		{
			StateName = jumpShoot;
		}
		else
		{
			Fall(fall);
			StateName = Jumping;/////
		}
	}
}

//Trạng thái lăn
void SamusAranState::RollState(Keyboard* key, bool isboom)
{
	jump = true;
	shoot = false;

	if (key->IsKeyDown(Dik_UP) || ( key->IsKeyDown(Dik_JUMP) && key->GIsKeyUp(Dik_DOWN) ))
	{
		StateName = Standing;
	}
	else if (key->IsKeyDown(Dik_SHOOT) && isboom)
	{
		StateName = rollShoot;
	}
	else
	{
		StateName = Rolling;/////
		FallRoll(fall);

	}
}

//Đứng nhìn lên
void SamusAranState::StandUpState(Keyboard* key)
{
	jump = true;
	fall = false; 
	shoot = false;

	Samus->SetVelocityY(-Gravity);

	if (key->IsKeyDown(Dik_UP))
	{
		if (key->IsKeyDown(Dik_JUMP))
		{
			StateName = Jumpingup;
		}
		else if (Samus->GetVelocity().x != 0)
		{
			StateName = Runningup;
		}
		else if (key->IsKeyDown(Dik_SHOOT))
		{
			StateName = standupShoot;
		}
		else StateName = Standingup;/////
	}
	else
	{
		StateName = Standing;
	}
}

//Chạy nhìn lên
void SamusAranState::RunUpState(Keyboard* key)
{
	jump = true;
	fall = false;
	shoot = false;

	if (key->IsKeyDown(Dik_UP))
	{
		if (key->IsKeyDown(Dik_LEFT) || key->IsKeyDown(Dik_RIGHT))
		{
			if (key->IsKeyDown(Dik_SHOOT))
			{
				StateName = runupShoot;
			}
			else if (key->IsKeyDown(Dik_JUMP))
			{
				StateName = Jumpingup;
			}
			else
			{
				fall = true;
				Fall(fall);
				StateName = Runningup;/////
				if (Samus->GetVelocity().y < -FallAc*2)
				{
					jump = false;
					Samus->SetVelocity(Samus->GetVelocity().x, -Gravity);
					StateName = Jumping;
				}
			}
		}
		else
		{
			StateName = Standingup;
		}
	}
	else	StateName = Running;
}

//Nhảy nhìn lên
void SamusAranState::JumpUpState(Keyboard* key)
{
	shoot = false;

	if (jump)
	{
		StartJump = Samus->GetPosition().y;
		Samus->SetVelocityY(JumpSpeed);
	}
	jump = false;

	if (key->GIsKeyUp(Dik_JUMP) || (Samus->GetPosition().y - StartJump) >= JumpMax)
		fall = true;

	if (key->IsKeyDown(Dik_UP) )
	{
		if (fall && Samus->GetVelocity().y == 0.0f)
		{
			StateName = Standingup;
		}
		else
		{
			if (key->IsKeyDown(Dik_SHOOT))
			{
				StateName = jumpupShoot;
			}
			else
			{
				Fall(fall);
				StateName = Jumpingup;//////
			}
		}
	}
	else	StateName = Jumping;
}

//Chạy và nhảy
void SamusAranState::RunJumpState(Keyboard* key)
{
	shoot = false;

	if (jump)
	{
		StartJump = Samus->GetPosition().y;
		Samus->SetVelocityY(JumpSpeed);
	}
	jump = false;

	if (key->GIsKeyUp(Dik_JUMP) || (Samus->GetPosition().y - StartJump) >= JumpMax)
		fall = true;

	if (fall && Samus->GetVelocity().y == 0.0f)
	{
			fall = false;

			if (Samus->GetVelocity().x != 0)
			{
				StateName = Running;
			}
			else
				StateName = Standing;
	}
	else
	{
		if (key->IsKeyDown(Dik_SHOOT))
		{
			StateName = jumpShoot;
		}
		else
		{
			Fall(fall);
			StateName = Runningjump;/////
		}
	}
}

//Trạng thái bắn
void SamusAranState::ShootState(Keyboard* key)
{
	//Rơi
	if (StateName == rollShoot)
		FallRoll(fall);
	else
		Fall(fall);

	if (timeShoot >= ShootTime)
	{
		//Cho bắn
		shoot = true;
		timeShoot = 0;

		switch (StateName)
		{
		case SamusAranState::standShoot:
			StateName = Standing;
			break;

		case SamusAranState::runShoot:
		case SamusAranState::runupShoot:
		{
			jump = true;
			fall = false;
			//Lấy trạng thái vẽ
			if (key->IsKeyDown(Dik_UP))
			{
				StateName = runupShoot;
			}
			else StateName = runShoot;
			//Điều kiện thoát
			if (key->GIsKeyUp(Dik_SHOOT) || Samus->GetVelocity().x == 0.0f)
				StateName = Running;
			else if (key->IsKeyDown(Dik_JUMP))
				StateName = Jumping;
			//Điều kiện rơi
			if (Samus->GetVelocity().y < -FallAc*2)
			{
				jump = false;
				fall = true;
				Samus->SetVelocity(Samus->GetVelocity().x, -Gravity);
				StateName = Jumping;
			}
			break;
		}

		case SamusAranState::jumpShoot:
		{
			if (key->GIsKeyUp(Dik_JUMP))
			{
				fall = true;
			}

			if (key->IsKeyDown(Dik_UP) || key->GIsKeyUp(Dik_SHOOT) || (Samus->GetVelocity().y <= 0.0f && Samus->GetVelocity().y >= -FallAc*2))
				StateName = Jumping;
		}
		break;

		case SamusAranState::rollShoot:
		{
			StateName = Rolling;
			break;
		}

		case SamusAranState::standupShoot:
			StateName = Standingup;
			break;

		case SamusAranState::jumpupShoot:
			StateName = Jumpingup;
			break;
		}
	}
	else shoot = false;
}

//Trạng thái chêt
void SamusAranState::DeadState()
{
	Samus->SetVelocity(0, 0);
	StateName = Dying;
}

//Trạng thái bị thương
void SamusAranState::BleedState(int side, int dame, bool Bleed)
{
	if (dame == 0)
		return;
	Side = side;
	bleed = Bleed;
	push = true;
	//Không mất máu dùng để boom đẩy
	if (!Bleed)
	{
		Samus->SetAllowDraw(true);
		velocityBleed.y = Gravity;
		timeBleed = 0.0f;
	}
	//Khi bị mất máu
	else
	{
		//Mất máu do quái đụng
		if (side != 0)
		{
			//Bất tử 1s
			if (timeBleed >= 1.0f)
			{
				timeBleed = 0.0f;
				velocityBleed.y = Gravity;
				Samus->SetHP(Samus->GetHP() - dame);
			}
		}
		//Mất máu do xuống lửa hole;
		else
		{
			//-Gravity để không nảy
			velocityBleed.y = -Gravity;
			//0.25s mới mất 1 lần
			if (timeBleed >= 0.25f)
			{
				timeBleed = 0.0f;
				Samus->SetHP(Samus->GetHP() - dame);
			}
		}
		//Kiểm tra SamusAran chết chưa

		if (Samus->GetHP() <= 0)
		{
			Samus->SetHP(0);
			Samus->SetVelocity(0, 0);
			StateName = Dying;
		}
	}	
}

void SamusAranState::Update(float gameTime, Keyboard* key)
{
	//trạng thái mất máu
	if (push)
	{
		timeBleed += gameTime;
		if (bleed)
		{
			//Mất máu thì nháy
			if (appear)
			{
				Samus->SetAllowDraw(true);
				appear = false;
			}
			else
			{
				Samus->SetAllowDraw(false);
				appear = true;
			}
		}
		//chỉnh Vận tốc
		velocityBleed.y = velocityBleed.y - 0.075f;
		//Đang bị đẩy
		if (velocityBleed.y > -Gravity && Samus->GetVelocity().y != 0.0f)
		{
			Samus->SetVelocityY(velocityBleed.y);
			if (Side > 0)
				Samus->SetVelocityX(-0.75);
			else if (Side < 0)
				Samus->SetVelocityX(0.75);
		}

		if (timeBleed > 2.0f)
		{
			bleed = false;
			push = false;
			Samus->SetAllowDraw(true);
		}
	}

	switch (StateName)
	{
	case SamusAranState::standShoot:
	case SamusAranState::runShoot:
	case SamusAranState::jumpShoot:
	case SamusAranState::rollShoot:
	case SamusAranState::standupShoot:
	case SamusAranState::runupShoot:
	case SamusAranState::jumpupShoot:
	{
		timeShoot += gameTime;
		break;
	}
	case SamusAranState::Appearing:
		timeapear += gameTime;
		break;
	default:
		break;
	}
}