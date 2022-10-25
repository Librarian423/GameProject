#include "Object.h"
#include "../Framework/Utils.h"

int Object::objCount = 0;

Object::Object()
{
	id = ++objCount;
	Init();
}

Object::~Object()
{
	Release();
}

int Object::GetObjId() const
{
	return id;
}

void Object::SetActive(bool active)
{
	enabled = active;
}

bool Object::GetActive() const
{
	return enabled;
}

void Object::Init()
{
	//hitbox.setFillColor(Color::Red);
	Reset();
}

void Object::Release()
{
}

void Object::Reset()
{
	enabled = true;
}

void Object::SetPos(const Vector2f& pos)
{
	position = pos;
	//Vector2f hitboxPos = { hitBoxRect.left, hitBoxRect.top + 50.f };
	//hitbox.setPosition(hitboxPos + pos);
}

const Vector2f& Object::GetPos() const
{
	return position;
}

void Object::Translate(Vector2f delta)
{
	SetPos(position + delta);
}

void Object::Update(float dt)
{

}

void Object::Draw(RenderWindow& window)
{
	
}
