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
	Animator animator;
	Types currState;
	Player* player;
	HitBox* itemHitbox;

	bool isHitBox;
	bool isKey;
	int value;

public:
	Item();
	virtual~Item();

	void Init(Types type);

	void SetType(Types newTypes);
	Types GetType() { return currState; }

	void Update(float dt);
	void Draw(RenderWindow& window);

	void SetPlayer(Player* p);
	void SetValue(int num) { value = num; }
	int GetValue() { return value; }
	void SetIsKey(bool key = true) { isKey = key; }

};

