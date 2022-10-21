#include "VertexArrayObj.h"

VertexArrayObj::VertexArrayObj()
	:texture(nullptr), origin(Origins::TL)
{
}

VertexArrayObj::~VertexArrayObj()
{
}

VertexArray& VertexArrayObj::GetVA()
{
	return vertexArray;
}

void VertexArrayObj::SetTexture(Texture* tex)
{
	texture = tex;
}

Texture* VertexArrayObj::GetTexture() const
{
	return texture;
}

FloatRect VertexArrayObj::GetGlobalBounds() const
{
	return vertexArray.getBounds();
}

void VertexArrayObj::SetOrigin(Origins newOrigin)
{
	FloatRect rect = vertexArray.getBounds();
	Vector2f prevPos(
		rect.width * ((int)origin % 3) * 0.5f, 
		rect.height * ((int)origin / 3) * 0.5f);
	Vector2f newPos(
		rect.width * ((int)newOrigin % 3) * 0.5f, 
		rect.height * ((int)newOrigin / 3) * 0.5f);
	Translate(prevPos - newPos);
	origin = newOrigin;
}

Origins VertexArrayObj::GetOrigin() const
{
	return origin;
}

void VertexArrayObj::SetPos(const Vector2f& pos)
{
	Vector2f delta = pos - position;

	for ( int i = 0; i < vertexArray.getVertexCount(); i++ )
	{
		vertexArray[i].position += delta;
	}
	position = pos;
}

const Vector2f& VertexArrayObj::GetPos() const
{
	return position;
}

void VertexArrayObj::Update(float dt)
{
}

void VertexArrayObj::Draw(RenderWindow& window)
{
	window.draw(vertexArray, texture);
}
