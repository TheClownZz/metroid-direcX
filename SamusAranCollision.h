#pragma once
#include "Collision.h"
#include "SamusAranState.h"
#include "Object.h"
#include "GameDefine.h"

using namespace Define;


class SamusAranCollision : public Collision
{
protected:
	Object* Samus;

	RECT Bound;

	SamusAranState::Statename Statebefore;
	SamusAranState::Statename NextState;

	bool standerror;

public:
	SamusAranCollision(Object* samus);
	SamusAranCollision();
	~SamusAranCollision();

	SamusAranState::Statename GetNextState();
	void SetNextState(SamusAranState::Statename state);
	SamusAranState::Statename GetStateBefore();
	void SetStateBefore(SamusAranState::Statename state);

	//Kiểm tra bound có thay đổi không
	RECT GetBound();
	void SetBound(SamusAranState::Statename state);

	void StandCollision();
	void RunCollision();
	void JumpCollision();
	void RollCollision();

	void CollisionFalse(RECT other);

};

