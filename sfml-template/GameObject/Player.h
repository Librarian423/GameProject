#pragma once
#include "SpriteObj.h"
#include "Slime.h"
#include "../Animation/Animator.h"

class HitBox;

class Player : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Attack,
	};
protected:
	//Sprite sprite;
	Animator animator;

	HitBox *playerHitbox;
	HitBox *attackHitbox;

	States currState;
	States prevState;

	float speed;
	Vector2f velocity;

	Vector2f direction;
	Vector2f lastDirection;
	Vector2f tempDirection;

	float timer;
	float attackTime;

	bool isHitBox;
public:
	Player() : currState(States::None), speed(500.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), timer(1.f), attackTime(0.8f), isHitBox(true) {}
	
	void Init();

	void SetState(States newState);

	void Update(float dt);
	//void UpdateInput(Event ev);
	void Draw(RenderWindow& window);
	void PlayAttack();
	void OnCompleteAttack();

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	void UpdateAttack(float dt);

	bool EqualFloat(float a, float b);

	HitBox* GetPlayerHitBox();
	HitBox* GetAttackHitbox();
	Vector2f GetPlayerDir() { return direction; }
};

