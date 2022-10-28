#pragma once
#include "SpriteObj.h"
#include "../Animation/Animator.h"

class Player;
class HitBox;

class Slime : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Dead,
	};
protected:
	Player* player;
	Animator animator;

	RectangleShape healthBar;

	HitBox* slimeHitbox;

	States currState;
	States prevState;

	float speed;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f dir;

	int slimeState;
	float moveTime;
	float hitTime;
	float getAttackTime;

	bool attack;

	int damage;
	int maxHp;
	int hp;
	float barScaleX;

	bool isHitBox;
public:
	Slime();
	virtual ~Slime();
	
	void Init(Player* player);

	void SetState(States newState);
	States GetState();

	void Update(float dt);
	void Draw(RenderWindow& window);

	void OnCompleteDead();
	bool EqualFloat(float a, float b);

	void SetHp(int num);
	void SetHpBar();
};

