#pragma once
#include <d3dx9.h>
#include "Keyboard.h"
#include "Animation.h"
#include "SamusAranState.h"
#include "GameDefine.h"
using namespace Define;

class SamusAranAnimation :public Animation
{
protected:
	D3DXVECTOR2 LocalGun;

	float change = 1.0f;
	float timechange = 0.0f;
public:
	SamusAranAnimation(InfoSprite* info) :Animation(info) {}
	SamusAranAnimation();
	~SamusAranAnimation();

	void Appear();//Xuất hiện
	void Stand(); //Đứng im
	void StandUp(); //Đứng giơ súng
	void Run(); //Di chuyển
	void Jump();//Rơi hoặc (nhảy không di chuyển)
	void RunJump(); //Tới nhảy
	void Roll();//Lăn
	void StandShoot();//Đứng bắn
	void RunShoot();//Vừa chạy vừa bắn
	void JumpShoot();//(Rơi hoặc "nhảy không di chuyển") và bắn
	void StandUpShoot();//Đứng giơ súng và bắn
	void RunUp();//Chạy giơ súng lên
	void JumpUp();//(Rơi hoặc "nhảy không di chuyển") giơ súng lên
	void JumpUpShoot();//(Rơi hoặc "nhảy không di chuyển") giơ súng lên và bắn
	void Die();//Chết

	D3DXVECTOR2 GetLocalGun(int Index);

	void SetTransRoll(SamusAranState::Statename state, float timechange);
	void SetPositionRunJump();

	void Update(float gameTime, Keyboard* key);
};

