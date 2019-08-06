#include "SamusAranCollision.h"



SamusAranCollision::SamusAranCollision(Object* samus)
{
	Samus = samus;
	Statebefore = SamusAranState::Standing;
	NextState = SamusAranState::Standing;

	standerror = false;
}

SamusAranCollision::SamusAranCollision()
{
}

SamusAranCollision::~SamusAranCollision()
{
}

//Lấy trạng thái tiếp theo
SamusAranState::Statename SamusAranCollision::GetNextState()
{
	return NextState;
}
void SamusAranCollision::SetNextState(SamusAranState::Statename state)
{
	NextState = state;
}
//Set trạng thái trước đó
SamusAranState::Statename SamusAranCollision::GetStateBefore()
{
	return Statebefore;
}
void SamusAranCollision::SetStateBefore(SamusAranState::Statename state)
{
	Statebefore = state;
}

//Lấy bound
RECT SamusAranCollision::GetBound()
{
	return Bound;
}

//Set bound cho Samus
void SamusAranCollision::SetBound(SamusAranState::Statename state)
{
	switch (state)
	{
	case SamusAranState::Appearing:
	{
		Bound = Samus->GetBound(0, 30);
		break;
	}

	case SamusAranState::Standing:
	case SamusAranState::Running:
	case SamusAranState::Standingup:
	case SamusAranState::Runningup:
	case SamusAranState::standShoot:
	case SamusAranState::runShoot:
	case SamusAranState::standupShoot:
	case SamusAranState::runupShoot:
	{
		standerror = false;

		if (Samus->GetHeight() != 30.0f)
		{
			Samus->SetPositionY(Samus->GetPosition().y + (30.0f - Samus->GetHeight()) / 2);
		}

		Bound = Samus->GetBound(12.0f, 30.0f);
		break;
	}

	case SamusAranState::Jumping:
	case SamusAranState::Jumpingup:
	case SamusAranState::Runningjump:
	case SamusAranState::jumpShoot:
	case SamusAranState::jumpupShoot:
	{
		Bound = Samus->GetBound(12.0f, 22.0f);
		break;
	}

	case SamusAranState::Rolling:
	case SamusAranState::rollShoot:
	{
		if (Samus->GetHeight() != 12.0f && standerror)
		{
			Samus->SetPositionY(Samus->GetPosition().y + (12.0f - Samus->GetHeight()) / 2);
			standerror = false;
		}

		Bound = Samus->GetBound(12.0f, 12.0f);
		break;
	}

	default:
		break;
	}

}

//Va chạm khi đứng
void SamusAranCollision::StandCollision()
{
}

//Va chạm khi chạy
void SamusAranCollision::RunCollision()
{
	if (Samus->GetVelocity().x == 0)
	{
		//Va chạm thì trạng trái đứng cái 2 Sate
		if (Statebefore == SamusAranState::Standingup)
			NextState = SamusAranState::Standingup;
		else
			NextState= SamusAranState::Standing;
		return;
	}
}

//va chạm khi nhảy
void SamusAranCollision::JumpCollision()
{
}

//Va chạm khi lăn
void SamusAranCollision::RollCollision()
{
}

//Xử lí lỗi khi 2 RECT đè nhau
void SamusAranCollision::CollisionFalse(RECT other)
{
	switch (NextState)
	{
	case SamusAranState::Standing:
	case SamusAranState::Running:
	case SamusAranState::Standingup:
	case SamusAranState::Runningup:
	case SamusAranState::standShoot:
	case SamusAranState::runShoot:
	case SamusAranState::standupShoot:
	case SamusAranState::runupShoot:
	{
		if (Statebefore == SamusAranState::Rolling)
		{
			standerror = true;
			NextState = SamusAranState::Rolling;
			Samus->SetVelocityY(0);
			break;
		}
		
	}

	default:
		break;
	}
}