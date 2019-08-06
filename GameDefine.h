#pragma once
#include "dinput.h"

namespace Define
{
	//Windows
	const float GameWidth = 256.0f;
	const float GameHeight = 240.0f;
	const float WinWidth = 768.0f;
	const float WinHeight = 720.0f;
	const float SpeedGame = 1.0f;

	//Control
	const int Dik_SELECT = DIK_SPACE;
	const int Dik_START = DIK_RETURN;
	const int Dik_LEFT = DIK_LEFT;
	const int Dik_RIGHT = DIK_RIGHT;
	const int Dik_UP = DIK_UP;
	const int Dik_DOWN = DIK_DOWN;
	const int Dik_JUMP = DIK_Z;
	const int Dik_SHOOT = DIK_X;

	//Path
		//Menu
	const char MenuPNG[] = "./Resource Files/Menu.png";
	const char MenuXML[] = "./Resource Files/Menu.xml";
		//Samus
	const char SamusAranNormalPNG[] = "./Resource Files/SamusAranNormal.png";
	const char SamusAranRocketPNG[] = "./Resource Files/SamusAranRocket.png";
	const char SamusAranVariaPNG[] = "./Resource Files/SamusAranVaria.png";
	const char SamusAranXML[] = "./Resource Files/SamusAran.xml";
		//Đạn Samus
	const char BulletPNG[] = "./Resource Files/Bullet.png";
	const char BulletXML[] = "./Resource Files/Bullet.xml";
		//Items
	const char ItemsPNG[] = "./Resource Files/Items.png";
	const char ItemsXML[] = "./Resource Files/Items.xml";
		//Enemy
	const char EnemyPNG[] = "./Resource Files/Enemy.png";
	const char EnemyXML[] = "./Resource Files/Enemy.xml";
		//Port
	const char PortPNG[] = "./Resource Files/Port.png";
	const char PortXML[] = "./Resource Files/Port.xml";
		//Boss
	const char BossPNG[] = "./Resource Files/Boss.png";
	const char BossXML[] = "./Resource Files/Boss.xml";
		//Map
	const char TileSetPNG[] = "./Resource Files/MetriodTileMap.png";
	const char MapXML[] = "./Resource Files/MetroidMap.tmx";
		//QuadTree
	const char QuadTreeXML[] = "./Resource Files/QuadTree.xml";
		//Sound
	const LPTSTR SoundTitle		= "./Resource Files/SoundMetroid/01 Title Screen .wav";
	const LPTSTR SoundEnter		= "./Resource Files/SoundMetroid/02 Enter_ Samus.wav";
	const LPTSTR SoundBrinstar	= "./Resource Files/SoundMetroid/03 Brinstar.wav";
	const LPTSTR SoundChozos	= "./Resource Files/SoundMetroid/04 Chozos.wav";
	const LPTSTR SoundEnd		= "./Resource Files/SoundMetroid/06 Ending.wav";
	const LPTSTR SoundPowerUp   = "./Resource Files/SoundMetroid/05 Power Up.wav";

		//Sound Effect
	const LPTSTR SoundRun		 	 = "./Resource Files/SoundMetroid/Effects/Run.wav";
	const LPTSTR SoundJump		 	 = "./Resource Files/SoundMetroid/Effects/Jump.wav";
	const LPTSTR SoundDying			 = "./Resource Files/SoundMetroid/Effects/Dying.wav";
	const LPTSTR SoundDie			 = "./Resource Files/SoundMetroid/Effects/Die.wav";

	const LPTSTR SoundFire		 	 = "./Resource Files/SoundMetroid/Effects/Fire.wav";
	const LPTSTR SoundIceFire	 	 = "./Resource Files/SoundMetroid/Effects/IceFire.wav";
	const LPTSTR SoundBoomFire	  	 = "./Resource Files/SoundMetroid/Effects/BoomFire.wav";
	const LPTSTR SoundRocketFire	 = "./Resource Files/SoundMetroid/Effects/RocketFire.wav";
	const LPTSTR SoundBoomBurst		 = "./Resource Files/SoundMetroid/Effects/BoomBurst.wav";

	const LPTSTR SoundPortCollison	 = "./Resource Files/SoundMetroid/Effects/PortCollison.wav";
	const LPTSTR SoundSamusCollision = "./Resource Files/SoundMetroid/Effects/SamusCollision.wav";
	const LPTSTR SoundEnemyCollision = "./Resource Files/SoundMetroid/Effects/EnemyCollision.wav";
	const LPTSTR SoundRipperCollision = "./Resource Files/SoundMetroid/Effects/RipperCollision.wav";

	const LPTSTR SoundItemHP		 = "./Resource Files/SoundMetroid/Effects/ItemHP.wav";
	const LPTSTR SoundItemRocket	 = "./Resource Files/SoundMetroid/Effects/ItemRocket.wav";


	//Delay 
	const float AppearDelay = 90;
	const float RunDelay = 3;
	const float JumpDelay = 3;
	const float RollDelay = 3;
	const float BulletDelay = 5;
	const float ItemDelay = 5;
	const float PortDelay = 30;
	const float ExplosiveDelay = 0.5;

	//Speed
	const float RunSpeed = 0.75f;
	const float JumpSpeed = 1.0f;
	const float Gravity = 1.0f;
	const float BulletSpeed = 2.0f;

	//Các giới hạn
	const float FallAc = 0.05f;
	const float JumpMin = 32.0f;
	const float JumpMax = 88.0f;
	const float SBulletMax = 48.0f;//quãng đường đạn bay
	const int BulletMax = 5;//số lượng đạn bắn tối đa
	const float ShootTime = 0.1f;

}