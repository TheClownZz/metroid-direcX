#pragma once
#include "Sprite.h"

class Animation : public Sprite
{
protected:
	float TimeCurrent;
	int start, end, IndexPause;
	bool Pause;

public:
	Animation(InfoSprite* info);
	Animation();
	~Animation();
	//Set thông số để vẽ 
	void SetFrame(D3DXVECTOR2 Position, bool Flip, int Delay, int Start, int End);

	int GetIndex();
	void SetIndex(int index);
	bool GetFlipFlag();
	int GetDelay();
	int GetStart();
	int GetEnd();
	void SetPause(bool pause);
	void SetPause(bool, int);
	void Update(float gameTime, Keyboard* key);
};
