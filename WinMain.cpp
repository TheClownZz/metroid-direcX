#include <Windows.h>
#include "Game.h"
#include "GameDefine.h"
using namespace Define;

int WINAPI WinMain(HINSTANCE Hins, HINSTANCE HIns, LPTSTR a, int c)
{
	MSG Msg;
	//Khai báo Object game
	Game game(Hins, WinWidth, WinHeight, "Metroid");
	if (!game.InitWD())
	{
		MessageBox(NULL, "Can't load Init Game", "Error", MB_OK);
		return 0;
	}

	//Load Data 
	game.InitDT();
	//Biến thời gian
	LARGE_INTEGER startTime;
	LARGE_INTEGER frequence;
	float Fps = SpeedGame / 60.0f;
	float gameTime = 0;
	LARGE_INTEGER endTime;
	float delay = 0;
	ZeroMemory(&Msg, sizeof(Msg));
	QueryPerformanceFrequency(&frequence);//Hàm trả gái trị đúng
	QueryPerformanceCounter(&endTime);

	//Render
	while (Msg.message != WM_QUIT)
	{
		if (PeekMessage(&Msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
			QueryPerformanceCounter(&startTime);
			QueryPerformanceCounter(&endTime);
			gameTime += ((float)endTime.QuadPart - (float)startTime.QuadPart) / (float)frequence.QuadPart;
			if (gameTime >= Fps)
			{
				game.Update(gameTime);
				game.Render();
				gameTime = 0;
			}
			else
			{
				Sleep(Fps - gameTime);
				gameTime = Fps;
			}

		}
	}
	//game.Delete();
	return 0;
}