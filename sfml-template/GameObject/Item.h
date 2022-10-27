#pragma once
#include "SpriteObj.h"
#include "../Animation/Animator.h"

class Player;
class HitBox;

class Item : public SpriteObj
{
public:
	enum class Types
	{
		None = -1,
		Coin,
		Potion,
		Key,
	};
protected:
	//Sprite sprite;
	Animator animator;
	Types currState;
	Player* player;
	HitBox* itemHitbox;

public:
	Item();
	virtual~Item();

	void Init();

	void SetType(Types newTypes);
	Types GetType() { return currState; }

	void Update(float dt);
	void Draw(RenderWindow& window);

	void SetPlayer(Player* p);


	bool EqualFloat(float a, float b);
};

