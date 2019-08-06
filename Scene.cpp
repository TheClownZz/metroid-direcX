#include "Scene.h"


Scene::Scene()
{
	SceneStage = TileScreen;
}


Scene::~Scene()
{
	delete sprite;
	delete MenuAnimation;
	delete sound;
	delete soundTitle;
	delete soundEnd;
	delete ObjManager;
}

//Load Data Game
void Scene::InitDT(Graphic* graphic)
{
	//sound
	sound = new Sound(graphic->GetHwnd());
	sound->Init_DirectSound();

	soundTitle = sound->LoadSound(SoundTitle);
	soundEnd = sound->LoadSound(SoundEnd);

	//tạo ảnh Menu
	sprite = new Sprite(graphic, MenuPNG);
	info_scene = new InfoSprite(MenuXML);
	MenuAnimation = new Animation(info_scene);
	positionMenu = D3DXVECTOR2(0, 0);
	sprite->SetPosition(positionMenu);
	timedelay = 0.0f;

	//Tạo GamePlay
	ObjManager = new ObjectManager();
	ObjManager->InitDT(graphic, sound);

	sound->LoopSound(soundTitle);
}

//Update Game sau khoảng thời gian gameTime
void Scene::Update(float gameTime, Keyboard* key)
{
	switch (SceneStage)
	{
	//Màn hình khởi động
	case Scene::TileScreen:
	{
		timedelay += gameTime;
		//sau 10s lặp lại
		if (timedelay >= 10.0f)
		{
			timedelay = 0.0f;
		}
		//Sau 5s hiện nhà phát hành
		else if (timedelay >= 5.0f)
		{
			MenuAnimation->SetFrame(positionMenu, false, 0, 2, 2);
		}
		//Hiệu ứng nhấp nháy titlescreen sau 3s
		else if (timedelay >= 3.0f)
		{
			MenuAnimation->SetFrame(positionMenu, false, 3, 0, 1);
		}
		//Không thì hiện bắt đầu
		else
		{
			MenuAnimation->SetFrame(positionMenu, false, 0, 0, 0);
		}
		//Nhấn Enter vào Menu
		if (key->IsKeyDown(Dik_START))
		{
			SceneStage = Menu;
			timedelay = 0.0f;
		}
		//Update
		MenuAnimation->Update(gameTime, key);
		break;
	}

	//Menu chọn Start
	case Scene::Menu:
	{
		timedelay += gameTime;
		MenuAnimation->SetFrame(positionMenu, false, 0, 3, 3);
		//Nhấn Enter vào Play
		if (key->IsKeyDown(Dik_START) && timedelay >= 0.25f)
		{
			sound->StopSound(soundTitle);
			sound->StopSound(soundEnd);
			ObjManager->Start();
			SceneStage = Play;
			timedelay = 0.0f;
		}
		//Update
		MenuAnimation->Update(gameTime, key);
		break;
	}

	//GamePlay
	case Scene::Play:
	{
		ObjManager->Update(gameTime, key);

		int end = ObjManager->End();
		//Bằng 1 là Samus chết chuyển scene gameover
		if (end == 1)
		{
			//delay 3s rồi kết thúc
			timedelay += gameTime;
			if (timedelay >= 3.0f)
			{
				SceneStage = Gameover;
				timedelay = 0.0f;
			}
		}
		//Bằng 2 là Boss cuối chết chuyển scene end
		else if (end == 2)
		{
			timedelay += gameTime;
			if (timedelay >= 3.0f)
			{
				sound->LoopSound(soundEnd);
				SceneStage = EndGame;
				timedelay = 0.0f;
			}
		}

		break;
	}

	//Samus chết
	case Scene::Gameover:
	{
		MenuAnimation->SetFrame(positionMenu, false, 0, 4, 4);
		//Nhấn Enter vào Play
		if (key->IsKeyDown(Dik_START))
		{
			SceneStage = Menu;
			timedelay = 0.5f;
		}
		//Update
		MenuAnimation->Update(gameTime, key);
		break;
	}

	//Phá đảo về nước
	case Scene::EndGame:
	{
		timedelay += gameTime;

		//Kết thúc trò chơi
		if (timedelay >= 6.0f)
		{
			MenuAnimation->SetFrame(positionMenu, false, 0, 7, 7);
			//Nhấn Enter vào Menu
			if (key->IsKeyDown(Dik_START) && timedelay >= 7.0f)
			{
				SceneStage = Menu;
				timedelay = 0.0f;
			}
		}
		//Giới thiệu về nhà sản xuất
		else if (timedelay >= 2.0f)
		{
			MenuAnimation->SetFrame(positionMenu, false, 0, 6, 6);
		}
		//Hiện ảnh kết thúc
		else
		{
			MenuAnimation->SetFrame(positionMenu, false, 0, 5, 5);
		}

		//Update
		MenuAnimation->Update(gameTime, key);
		break;
	}
	default:
		break;
	}
	
}

//Vẽ Object lên màn hình
void Scene::Render()
{
	if (SceneStage == Play)
		ObjManager->Render();
	else
	{
		sprite->SetRect(MenuAnimation->GetRect());
		sprite->Render();
	}
}