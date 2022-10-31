#pragma once
#include "SpriteObj.h"
#include "../Framework/Utils.h"
#include <list>

class Player;
class HitBox;
class Slime;

class VertexArrayObj : public SpriteObj
{
protected:
	VertexArray vertexArray;
	Texture* texture;
	Origins origin;

	std::list<HitBox*> wallHitboxList;
	HitBox* wallHitbox;

	bool isHitBox;
public:
	VertexArrayObj();
	~VertexArrayObj();

	void Init();

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

	void MakeWallHitBox(Vector2f pos);
	
	std::list<HitBox*>GetHitBoxList();
};

