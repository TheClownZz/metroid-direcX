#include "Viewport.h"



Viewport::Viewport()
{
}

Viewport::Viewport(float x, float y)
{
	this->positionWorld.x =x;
	this->positionWorld.y =y;
	//Mặc định là kích thước game
	this->Width = GameWidth;
	this->Height = GameHeight;
	MoveZoom = { 0,	1440, 1280, 1200 };

	MoveScene = 0;
}

Viewport::Viewport(float x, float y, float width, float height)
{
	this->positionWorld.x = x;
	this->positionWorld.y = y;
	this->Width = width;
	this->Height = height;
}

Viewport::~Viewport()
{
}

//Get set vị trí Viewport
D3DXVECTOR2 Viewport::GetPosition()
{
	return positionWorld;
}
void Viewport::SetPosition(D3DXVECTOR2 Position)
{
	positionWorld = Position;
}
void Viewport::SetPositionX(float x)
{
	positionWorld.x = x;	
}
void Viewport::SetPositionY(float y)
{
	positionWorld.y = y;
}
RECT Viewport::GetScene()
{
	return MoveZoom;
}

//Lấy chiều dài rộng của Viewport
float Viewport::GetWidth()
{
	return this->Width;
}
float Viewport::GetHeight()
{
	return this->Height;
}

//Lấy vị trí của ViewPort từ vị trị object cần vẽ theo hệ trục left-top
D3DXVECTOR2 Viewport::GetPositionViewport_Left_Top(D3DXVECTOR2 Position)
{
	return Position-positionWorld;
}

//Lấy vị trí của ViewPort từ vị trị object cần vẽ theo hệ đề các
D3DXVECTOR2 Viewport::GetPositionViewport(D3DXVECTOR2 Position)
{
	//int để vị trí ViewPort không lệch pixel
	float x = positionWorld.x - (int)positionWorld.x;
	float y = positionWorld.y - (int)positionWorld.y;
	D3DXVECTOR3* position = &D3DXVECTOR3(Position.x + x, Position.y + y, 0);

	D3DXMATRIX mt;
	D3DXVECTOR4 posViewport;

	//đổi ma trận chuẩn bị nhân lấy tọa độ mới
	D3DXMatrixIdentity(&mt);
	mt._22 = -1.0f;
	mt._41 = -positionWorld.x;
	mt._42 = positionWorld.y;

	D3DXVec3Transform(&posViewport, position, &mt);

	return D3DXVECTOR2(posViewport.x, posViewport.y);
}

//Kiểm tra 1 rect có ở trong màn hình không
bool Viewport::isContains(RECT rect)
{
	if (rect.right < positionWorld.x)
		return false;
	if (rect.bottom > positionWorld.y)
		return false;
	if (rect.left > positionWorld.x + Width)
		return false;
	if (rect.top < positionWorld.y - Height)
		return false;

	return true;
}

//Kích thước RECT màn hình tính trong World
RECT Viewport::GetBoundViewport()
{
	RECT rect;

	rect.left = positionWorld.x;
	rect.top = positionWorld.y;
	rect.bottom = rect.top - Height;
	rect.right = rect.left + Width;

	return rect;
}

//Update theo 1 đối tượng
void Viewport::Update(float gameTime, Keyboard* key, D3DXVECTOR2 &posobject, std::vector <RECT> SceneZoom)
{
	if (key->IsKeyDown(DIK_0) || posobject == D3DXVECTOR2(640, 1266))
	{
		posobject = D3DXVECTOR2(640, 1266);
		MoveZoom = { 0,	1440, 1280, 1200 };
		return;
	}
	else if (key->IsKeyDown(DIK_1))
	{
		posobject = D3DXVECTOR2(2745, 1220);
		MoveZoom = { 2560, 4560 , 2816 , 960 };
		return;
	}
	else if (key->IsKeyDown(DIK_2))
	{
		posobject = D3DXVECTOR2(5685, 3010);
		MoveZoom = { 5632, 3120 , 7424 , 2880 };
		return;
	}
	else if (key->IsKeyDown(DIK_3))
	{
		posobject = D3DXVECTOR2(3400, 2050);
		MoveZoom = { 3328, 2160 , 5376 , 1920 };
		return;
	}
	else if (key->IsKeyDown(DIK_4))
	{
		posobject = D3DXVECTOR2(3380, 3970);
		MoveZoom = { 3328, 4080 , 4864 , 3840 };
		return;
	}

	//Rơi vào hang boss
	if (posobject.y + 32 < MoveZoom.bottom)
	{
		posobject = D3DXVECTOR2(2888, 1248);
		MoveZoom = { 2816, 1440 , 3200 , 1200 };
		return;
	}

	//Kiểm tra View
	if (posobject.x > MoveZoom.right)
	{
		//Tìm Scene kế tiếp
		for (size_t i = 0; i < SceneZoom.size(); i++)
				if (!(posobject.x < SceneZoom[i].left || posobject.x > SceneZoom[i].right || 
					posobject.y > SceneZoom[i].top || posobject.y < SceneZoom[i].bottom))
				{
					newX = SceneZoom[i].left;
					MoveZoom = SceneZoom[i];
					break;
				}
		MoveScene = 1;
		MoveZoom.left = positionWorld.x;
	}
	else if (posobject.x < MoveZoom.left)
	{
		//Tìm Scene kế tiếp
		for (size_t i = 0; i < SceneZoom.size(); i++)
				if (!(posobject.x < SceneZoom[i].left || posobject.x > SceneZoom[i].right ||
					posobject.y > SceneZoom[i].top || posobject.y < SceneZoom[i].bottom))
				{
					newX = SceneZoom[i].right;
					MoveZoom = SceneZoom[i];
					break;
				}
		MoveScene = -1;
		MoveZoom.right = positionWorld.x + Width;
	}

	//Di chuyển View
	if (MoveScene == 1)
	{
		MoveZoom.left += 120 * gameTime;
		if (MoveZoom.left >= newX)
		{
			MoveZoom.left = newX;
			MoveScene = 0;
		}
	}
	else if (MoveScene == -1)
	{
		MoveZoom.right -= 120 * gameTime;
		if (MoveZoom.right <= newX)
		{
			MoveZoom.right = newX;
			MoveScene = 0;
		}
	}
	else
	{
		//trục X
		if (posobject.x - positionWorld.x <= 112)
			positionWorld.x = posobject.x - 112;
		else if (posobject.x - positionWorld.x >= 144)
			positionWorld.x = posobject.x - 144;

		//trục Y
		if (positionWorld.y - posobject.y <= 112)
			positionWorld.y = posobject.y + 112;
		else if (positionWorld.y - posobject.y >= 144)
			positionWorld.y = posobject.y + 144;
	}

	if (positionWorld.x < MoveZoom.left)
		positionWorld.x = MoveZoom.left;
	else if (positionWorld.x > MoveZoom.right - Width)
		positionWorld.x = MoveZoom.right - Width;

	if (positionWorld.y < Height + MoveZoom.bottom)
		positionWorld.y = Height + MoveZoom.bottom;
	else if (positionWorld.y > MoveZoom.top)
		positionWorld.y = MoveZoom.top;
}