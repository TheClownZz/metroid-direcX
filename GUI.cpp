#include "GUI.h"
#include "string.h"
#pragma warning(disable : 4996)

GUI::GUI(Graphic* graphic)
{
	//Text
	gui = new DrawString(graphic, 12);
	bulletgui = new Sprite(graphic, BulletPNG);
	bulletgui->SetPosition(D3DXVECTOR2(32, 47));
	RECT rectguimr = {42, 8, 53, 16};
	bulletgui->SetRect(rectguimr);
	bulletgui->Flip(true);
	//Hiển thị HP dùng rTank là hiển thị bình máu
	strcpy(textHP, "EN ..");
	rTank = { 18 , 25, 55, 35 };
	rHP = { 15, 35, 40, 45 };
	r2 = { 40, 35, 55, 45 };
	//MissileRocket
	r4 = { 35, 45, 55, 55 };
}
GUI::GUI()
{
}

GUI::~GUI()
{
	delete gui;
	delete bulletgui;
}

//Vẽ
void GUI::Render(SamusAran* Samus)
{
	//Hiển thị HP
	char Hp[5];
	char tank[20];
	strcpy(tank, " ");
	for (int i = Samus->Tank - 1; i > 0; i--)
	{
		//Những bình tank còn là O hết là U
		if (i - 1 < Samus->GetHP() / 100)
			strcat_s(tank, "O ");
		else
			strcat_s(tank, "U ");
	}
	itoa(Samus->GetHP() % 100, Hp, 10);

	gui->Render(tank, rTank, D3DCOLOR_XRGB(0, 0, 255));
	gui->Render(textHP, rHP, D3DCOLOR_XRGB(0, 0, 255));
	gui->Render(Hp, r2, D3DCOLOR_XRGB(255, 255, 255));

	//Ăn rocket mới hiện Chỉ số Rocket
	if (Samus->FlagItems.rocket)
	{
		char *Mr = new char[5];
		itoa(Samus->rocketbullet, Mr, 10);

		bulletgui->Render();
		gui->Render(Mr, r4, D3DCOLOR_XRGB(255, 255, 255));
	}
}