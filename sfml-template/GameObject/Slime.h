#pragma once
#include "../Animation/Animator.h"

class Slime
{
	enum class States
	{
		None = -1,
		Idle,
		Move,
	};
protected:
	Sprite sprite;
	Animator animator;

	States currState;
	States prevState;

	float speed;
	Vector2f direction;
	Vector2f lastDirection;

	int slimeState;
	float moveTime;

public:
	Slime() : currState(States::None), speed(200.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), slimeState(0), moveTime(0.f) {}
	
	void Init();

	void SetState(States newState);

	void Update(float dt);
	void Draw(RenderWindow& window);

	void UpdateIdle(float dt);
	void UpdateMove(float dt);

	void PlayIdle();
	void PlayMove();

	bool EqualFloat(float a, float b);
};

