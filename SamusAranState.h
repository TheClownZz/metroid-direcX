#pragma once
#include <d3dx9.h>
#include "Keyboard.h"
#include "Object.h"
#include "GameDefine.h"

using namespace Define;

class SamusAranState
{
protected:
	Object* Samus;
	bool appear, jump, allowjump, fall, shoot, bleed, push;
	float StartJump, timeShoot, timeBleed, timeapear;

	D3DXVECTOR2 velocityBleed;
	int Side;
public:
	enum Statename
	{
		Appearing,//Xuất hiện
		Standing, //Đứng im
		Running, //Di chuyển
		Jumping,//Rơi hoặc (nhảy không di chuyển)
		Rolling,//Lăn
		Standingup,//Nhìn lên
		Runningup,//chạy nhìn lên
		Jumpingup,//nhảy nhìn lên
		Runningjump,//chạy và nhảy

		standShoot,
		runShoot,
		jumpShoot,
		rollShoot,
		standupShoot,
		runupShoot,
		jumpupShoot,

		Dying	//chết
	};
	Statename StateName;

	SamusAranState(Object* samus);
	SamusAranState();
	~SamusAranState();

	void MoveX(Keyboard* key);//Kiểm tra vận tốc
	void Fall(bool &fall);//Rơi
	void FallRoll(bool &fall);
	void AppearState(Keyboard* key, int Index);
	void StandState(Keyboard* key, bool isroll);
	void RunState(Keyboard* key);
	void JumpState(Keyboard* key);
	void RollState(Keyboard* key, bool isboom);
	void StandUpState(Keyboard* key);
	void RunUpState(Keyboard* key);
	void JumpUpState(Keyboard* key);
	void RunJumpState(Keyboard* key);
	void ShootState(Keyboard* key);
	void BleedState(int side, int dame, bool bleed);
	void DeadState();

	void SetFall(bool);
	float GetJumpHeight();
	Statename GetState();
	void SetState(Statename State);

	bool GetShoot();
	bool GetJump();

	void Update(float gameTime, Keyboard* key);
};

