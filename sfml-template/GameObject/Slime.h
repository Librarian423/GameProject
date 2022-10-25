#pragma once
#include "SpriteObj.h"
#include "../Animation/Animator.h"

class Player;

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

	States currState;
	States prevState;

	float speed;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f dir;

	int slimeState;
	float moveTime;
	float hitTime;
	float deleteTime;

	bool attack;

	int damage;
	int hp;

public:
	Slime() : currState(States::None), speed(50.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), slimeState(0), moveTime(0.f), hitTime(1.f), deleteTime(1.f), attack(true), damage(1), hp(2) {}
	
	void Init(Player* player);

	void SetState(States newState);
	States GetState();

	void Update(float dt);
	void Draw(RenderWindow& window);

	void UpdateIdle(float dt);
	void UpdateMove(float dt);

	void PlayIdle();
	void PlayMove();

	void OnCompleteDead();
	bool EqualFloat(float a, float b);
};

