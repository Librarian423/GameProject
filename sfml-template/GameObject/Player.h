#pragma once
#include "../Animation/Animator.h"

class Player
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Jump,
	};
protected:
	Sprite sprite;
	Animator animator;

	States currState;

	float speed;
	Vector2f direction;
	Vector2f lastDirection;

public:
	Player() : currState(States::None), speed(200.f), direction(1.f, 0.f), lastDirection(1.f, 0.f) {}
	void Init();

	void SetState(States newState);

	void Update(float dt);
	void UpdateInput(Event ev);
	void Draw(RenderWindow& window);

	void OnCompleteJump();

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	void UpdateJump(float dt);

	bool EqualFloat(float a, float b);
};

