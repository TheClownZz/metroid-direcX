#include "SamusAranAnimation.h"



SamusAranAnimation::SamusAranAnimation()
{
}

SamusAranAnimation::~SamusAranAnimation()
{
}

//Xuất hiện
void SamusAranAnimation::Appear()
{
	delay = AppearDelay;
	start = 0; end = 3;
}

//Đứng im
void SamusAranAnimation::Stand()
{
	delay = 0;
	start = 4; end = 4;
}

//Đứng giơ súng
void SamusAranAnimation::StandUp()
{
	delay = 0;
	start = 5; end = 5;
}

//Di chuyển
void SamusAranAnimation::Run()
{
	delay = RunDelay;
	start = 6; end = 8;
}

//Rơi hoặc (nhảy không di chuyển)
void SamusAranAnimation::Jump()
{
	delay = 0;
	start = 9; end = 9;
}

//Tới nhảy
void SamusAranAnimation::RunJump()
{
	delay = JumpDelay;
	start = 10; end = 13;
}

//Lăn
void SamusAranAnimation::Roll()
{
	delay = RollDelay;
	start = 14; end = 17;
}

//Đứng bắn
void SamusAranAnimation::StandShoot()
{
	delay = 0;
	start = 18; end = 18;
}

//Vừa chạy vừa bắn
void SamusAranAnimation::RunShoot()
{
	delay = RunDelay;
	start = 19; end = 21;
}

//(Rơi hoặc "nhảy không di chuyển") và bắn
void SamusAranAnimation::JumpShoot()
{
	delay = 0;
	start = 22; end = 22;
}

//Đứng giơ súng và bắn
void SamusAranAnimation::StandUpShoot()
{
	delay = 0;
	start = 23; end = 23;
}

//Chạy giơ súng lên
void SamusAranAnimation::RunUp()
{
	delay = RunDelay;
	start = 24; end = 26;
}

//(Rơi hoặc "nhảy không di chuyển") giơ súng lên
void SamusAranAnimation::JumpUp()
{
	delay = 0;
	start = 27; end = 27;
}

//(Rơi hoặc "nhảy không di chuyển") giơ súng lên và bắn
void SamusAranAnimation::JumpUpShoot()
{
	delay = 0;
	start = 28; end = 28;
}

void SamusAranAnimation::Die()
{
	delay = ExplosiveDelay;
	start = 29; end = 34;
}

//Lấy tọa độ cây súng so với vị trí vẽ
D3DXVECTOR2 SamusAranAnimation::GetLocalGun(int Index)
{
	if (FlipFlag)
		LocalGun.x = -InfoTexture->GetLocalGun(Index).x;
	else
		LocalGun.x = InfoTexture->GetLocalGun(Index).x;

	LocalGun.y = InfoTexture->GetLocalGun(Index).y;

	return LocalGun;
}

//Chỉnh trans cho Roll 
void SamusAranAnimation::SetTransRoll(SamusAranState::Statename state, float Timechange)
{
	if (state == SamusAranState::Rolling && timechange >= Timechange)
	{
		transform.y += change;
		change = -change;
		timechange = 0;
	}
}

//Chỉ cho RunJump
void SamusAranAnimation::SetPositionRunJump()
{
	if (start >= 10 && end <= 13)
		position.y += 12;
}

void SamusAranAnimation::Update(float gameTime, Keyboard* key)
{
	Animation::Update(gameTime, key);
	timechange += gameTime;
}
