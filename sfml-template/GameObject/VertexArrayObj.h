#pragma once
//#include "Object.h"
#include "SpriteObj.h"
#include "../Framework/Utils.h"

class Player;

class VertexArrayObj : public SpriteObj
{
protected:
	VertexArray vertexArray;
	Texture* texture;
	Origins origin;

	Player* player;
public:
	VertexArrayObj();
	~VertexArrayObj();

	VertexArray& GetVA();
	void SetTexture(Texture* tex);
	Texture* GetTexture()const;
	FloatRect GetGlobalBounds() const;

	void SetOrigin(Origins newOrigin);
	Origins GetOrigin() const;

	virtual void SetPos(const Vector2f& pos);
	virtual const Vector2f& GetPos() const;

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

