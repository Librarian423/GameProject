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
	Slime() : currState(States::None), speed(50.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), slimeState(0), moveTime(0.f), hitTime(1.f), getAttackTime(1.f), attack(true), damage(1), hp(10), maxHp(10), barScaleX(60.f), isHitBox(true) {}
	
	void Init(Player* player);

	void SetState(States newState);
	States GetState();

	void Update(float dt);
	void Draw(RenderWindow& window);

	void PlayIdle();
	void PlayMove();

	void OnCompleteDead();
	bool EqualFloat(float a, float b);

	void SetHp(int num);
	void SetHpBar();
};

