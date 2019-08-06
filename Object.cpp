#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

void Object::New()
{
}

bool Object::GetFlipFlag()
{
	return FlipFlag;
}
void Object::SetFlipFlag(bool flag)
{
	FlipFlag = flag;
}
//Khung va chạm
RECT Object::GetBound()
{
	return bound;
}
void Object::SetBound(RECT bound)
{
	this->bound = bound;
}
//position ở x, y ở giữa
RECT Object::GetBound(float width, float height)
{
	Width = width;
	Height = height;
	bound.left = position.x - width / 2;
	bound.right = bound.left + width;
	bound.top = position.y + height/2;
	bound.bottom = bound.top - height;
	return bound;
}

//Thông tin frame
float Object::GetWidth()
{
	return this->Width;
}
void Object::SetWidth(float width)
{
	this->Width = width;
}
float Object::GetHeight()
{
	return this->Height;
}
void Object::SetHeight(float height)
{
	this->Height = height;
}


//Vị trí
D3DXVECTOR2 Object::GetPosition()
{
	return position;
}
D3DXVECTOR2 Object::GetPositionStart()
{
	return positionStart;
}
void Object::SetPosition(D3DXVECTOR2 Position)
{
	position = Position;
}
void Object::SetPositionStart(D3DXVECTOR2 Position)
{
	positionStart = Position;
}
void Object::SetPositionX(float x)
{
	position.x = x;
}
void Object::SetPositionY(float y)
{
	position.y = y;
}

//Vận tốc
D3DXVECTOR2 Object::GetVelocity()
{
	return velocity;
}
void Object::SetVelocity(D3DXVECTOR2 Velocity)
{
	velocity = Velocity;
}
void Object::SetVelocityX(float x)
{
	velocity.x = x;
}
void Object::SetVelocityY(float y)
{
	velocity.y = y;
}
void Object::SetVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}

//Dịch chuyển
D3DXVECTOR2 Object::GetTransform()
{
	return transform;
}
void Object::SetTransform(D3DXVECTOR2 Transform)
{
	transform = Transform;
}
void Object::SetTransform(float x, float y)
{
	transform.x = x;
	transform.y = y;
}

//Góc
float Object::GetAngle()
{
	return angle;
}
void Object::SetAngle(float Angle)
{
	angle = Angle;
}

//Khoảng cách
D3DXVECTOR2 Object::Distance(float time)
{
	return velocity * 100 * time;
}


//cho phép vẽ
void Object::SetAllowDraw(bool allowDraw)
{
	AllowDraw = allowDraw;
}
bool Object::GetAllowDraw()
{
	return AllowDraw;
}

//Máu
void Object::SetHP(int hp)
{
	HP = hp;
}
int Object::GetHP()
{
	return HP;
}

//Máu
void Object::SetDamage(int damage)
{
	Damage = damage;
}
int Object::GetDamage()
{
	return Damage;
}

void Object::ChangeSkin(const char* pathpng)
{
	sprite->SetTexture(pathpng);
}

void Object::LockAnimation(bool Lock, int index)
{

}
void Object::ChangeAnimation()
{

}

//Kiểm tra chết chưa
bool Object::Die()
{
	if (HP <= 0)
		return true;
	else
		return false;
}

void Object::Update(float gameTime, Keyboard* key)
{
	position += velocity *gameTime * 100;
}
void Object::OnCollision(Object *obj, D3DXVECTOR2 distance)
{

}
void Object::Render(Viewport* viewport)
{
}
RECT Object::GetBoard(D3DXVECTOR2 distance)
{
	return Collision::GetBoard(this->GetBound(), distance);
}
Object::tag Object::GetTag(string name)
{
	//if((name=="HpItem")||(name=="RocketItem")||(name== "Marimaru")||(name=="Rocket")||(name== "Icebean")
	//	|| (name == "Boom" )|| (name == "Longbean") || (name == "Tank") || (name == "Varia"))
	if (name == "Wall")
	{
		return tag::Wall;
	}
	if ((name == "Scene"))
	{
		return tag::Scene;
	}
	if (name == "Hole")
	{
		return tag::Hole;
	}
	if ((name == "PortItem") || (name == "PortRock") || (name == "PortYellow") || (name == "PortBlue"))
	{
		return tag::Port;
	}
	if ((name == "Zoomer") || (name == "Skree") || (name == "Ripper")
		|| (name == "Rio") || (name == "Waver") || (name == "Zeb"))
	{
		return tag::Enemy;
	}
	return tag::Item;
}

void Object::SetTag(Object::tag name)
{
	Tag = name;
}
