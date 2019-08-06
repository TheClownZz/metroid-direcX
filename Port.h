#pragma once
#include "SamusAran.h"
#include "Animation.h"
#include "GameDefine.h"
#include "Collision.h"
#include"InfoMap.h"
using namespace Define;

class Port : public Object
{
protected:
	Animation* AnimationPort;
	SamusAran *Samus;
	InfoMap *info;

	int data;
	float timeclose;
	bool Open, SceneMove, slip;
	RECT BoundMove;

	//Sound
	GSound *soundportopen;
	GSound *soundportclose;

public:
	enum Porttype
	{
		PortItem,
		PortRock,
		PortYellow,
		PortBlue
	};
	Porttype PortType;

	Port();
	Port(InfoMap *Info, SamusAran *samus, Sprite* sprite_port, InfoSprite*info_port, Sound* sound);
	~Port();

	void ChangeAnimation(Keyboard* key);
	void  OnCollision();
	void New();
	void NewPort(D3DXVECTOR2 pos, Porttype type);
	static Porttype GetType(string name);
	void Update(float gameTime, Keyboard *key);
	void Render(Viewport *view);
};

