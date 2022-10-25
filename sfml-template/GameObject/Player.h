#pragma once
#include "SpriteObj.h"
#include "../Animation/Animator.h"

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

	States currState;
	States prevState;

	float speed;
	//float accelation;
	//float deaccelation;
	Vector2f velocity;

	Vector2f direction;
	Vector2f lastDirection;
	Vector2f tempDirection;

public:
	Player() : currState(States::None), speed(500.f), direction(1.f, 0.f), lastDirection(1.f, 0.f) {}//, accelation(500), deaccelation(500)
	
	void Init();

	void SetState(States newState);

	void Update(float dt);
	void UpdateInput(Event ev);
	void Draw(RenderWindow& window);
	void PlayAttack();
	void OnCompleteAttack();

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	void UpdateAttack(float dt);

	bool EqualFloat(float a, float b);

	Vector2f GetPlayerDir() { return direction; }
};

