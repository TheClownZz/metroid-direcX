#pragma once
#include "DrawString.h"
#include "Graphic.h"
#include "SamusAran.h"

class GUI
{
protected:
	DrawString *gui;
	Sprite* bulletgui;
	RECT rHP, r2, r4, rTank;
	char textMR[10];
	char textHP[10];
public:
	GUI(Graphic* graphic);
	GUI();
	~GUI();
	void Render(SamusAran* Samus);

};

