#pragma once
#include "Viewport.h"
#include "Sound.h"
#include "Sprite.h"
#include "DrawLine.h"
#include "Collision.h"
class Object
{
protected:
	Sprite* sprite;
	Sound* sound;
	float Width, Height;
	bool FlipFlag, AllowDraw, lock;
	D3DXVECTOR2 position; //vị trí vẽ
	D3DXVECTOR2 velocity;//Vector vận tốc
	D3DXVECTOR2 positionStart;
	D3DXVECTOR2 transform;
	float angle;
	RECT bound;
	int HP = 0, Damage = 0;
	string name;
	int limit;
public:
	int id;
	enum tag
	{
		Wall,//Tường
		Enemy,//Địch
		Port,//Cổng
		Hole,//rơi xuống là mất máu và làm chậm
		Scene,//Cái này để ra 1 list cho tui làm port
		Item,//item
		player,
		bullet,
		Boss
	}Tag;

	Object();
	~Object();
	static tag GetTag(string name);
	void SetTag(tag name);
	virtual bool GetFlipFlag();
	virtual void SetFlipFlag(bool flag);
	//Khung va chạm
	virtual RECT GetBound();
	virtual RECT GetBound(float width, float height);
	virtual void SetBound(RECT bound);

	//Thông tin frame
	virtual float GetWidth();
	virtual void SetWidth(float width);
	virtual float GetHeight();
	virtual void SetHeight(float height);

	//Vị trí
	virtual D3DXVECTOR2 GetPosition();
	virtual D3DXVECTOR2 GetPositionStart();
	virtual void SetPosition(D3DXVECTOR2 Position);
	virtual void SetPositionStart(D3DXVECTOR2 Position);
	virtual void SetPositionX(float x);//lấy vị trí từ bound sau khi va chạm
	virtual void SetPositionY(float y);

	//Vận tốc
	virtual D3DXVECTOR2 GetVelocity();
	virtual void SetVelocity(D3DXVECTOR2 Velocity);
	virtual void SetVelocityX(float x);
	virtual void SetVelocityY(float y);
	virtual void SetVelocity(float x, float y);

	//Dịch chuyển
	virtual D3DXVECTOR2 GetTransform();
	virtual void SetTransform(D3DXVECTOR2 Transform);
	virtual void SetTransform(float x, float y);

	//Góc
	virtual float GetAngle();
	virtual void SetAngle(float Angle);

	//Tính khoảng cách trong khoảng time
	virtual D3DXVECTOR2 Distance(float time);

	//cho phép vẽ
	virtual void SetAllowDraw(bool allowDraw);
	virtual bool GetAllowDraw();

	//Máu
	virtual void SetHP(int hp);
	virtual int GetHP();
	//Damage
	virtual void SetDamage(int damage);
	virtual int GetDamage();

	virtual void ChangeSkin(const char* pathpng);
	virtual void LockAnimation(bool lock, int index);
	virtual void ChangeAnimation();

	virtual bool Die();
	virtual void Update(float gameTime, Keyboard* key);
	virtual void Render(Viewport* viewport);
	// va cham
	virtual void OnCollision(Object *obj, D3DXVECTOR2 distance);
	virtual void OnCollision(Object *obj) {}
	virtual void OnCollision() {}
	RECT GetBoard(D3DXVECTOR2 distance);
	virtual void SetName(string name) { this->name = name; }
	virtual string GetName() { return name; }
	virtual void AddVelocity(float x, float y)
	{
		velocity.x += x;
		velocity.y += y;
	}
	virtual void New();
};

