#include "Object.h"

int Object::objCount = 0;

Object::Object()
	:isDevMod(false)
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
	hitbox.setFillColor(Color::Red);
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
	hitbox.setPosition(position);
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
	if ( isDevMod )
	{
		window.draw(hitbox);
	}
}

void Object::SetHitbox(const FloatRect rect)
{
	/*Vector2f hitboxPos = GetPos();

	hitboxPos.x -= rect.left;
	hitboxPos.y -= rect.top;*/

	hitbox.setPosition(position);
	hitbox.setSize({ rect.width, rect.height });
}
