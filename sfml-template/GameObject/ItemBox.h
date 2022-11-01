#pragma once
#include "SpriteObj.h"
#include "../Animation/Animator.h"
#include <list>

class Player;
class HitBox;

class ItemBox : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Open,
	};

protected:
	Player* player;
	Animator animator;
	HitBox* itemboxHitbox;

	States currState;
	
	float eraseTimer;

	bool isBoxOpen;
	bool isHitBox;

public:
	ItemBox();
	virtual ~ItemBox();

	void Init();

	void SetPlayer(Player* player);
	void SetState(States newState);

	void Update(float dt);
	void Draw(RenderWindow& window);

	void OnCompleteOpen();
	void SetBoxFalse(bool isTrue = false);
};

