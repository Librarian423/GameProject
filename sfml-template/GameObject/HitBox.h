#pragma once
#include "Object.h"

class HitBox : public Object
{
protected:
	RectangleShape hitbox;
	FloatRect hitBoxRect;

public:
	HitBox();
	virtual ~HitBox();

	virtual void SetActive(bool active);
	virtual bool GetActive() const;

	virtual void Init();
	virtual void Release();

	virtual void Reset();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	virtual void SetPos(const Vector2f& pos);
	virtual const Vector2f& GetPos() const;
	virtual void Translate(const Vector2f delta);

	void SetHitbox(const FloatRect rect);
	const RectangleShape& GetHitbox() const;

	virtual	void SetDevMode(bool dev);
};

