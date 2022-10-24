#pragma once
#include "SpriteObj.h"
#include "../Animation/Animator.h"

class Player;

class Slime : public SpriteObj
{
	enum class States
	{
		None = -1,
		Idle,
		Move,
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

public:
	Slime() : currState(States::None), speed(20.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), slimeState(0), moveTime(0.f) {}
	
	void Init(Player* player);

	void SetState(States newState);

	void Update(float dt);
	void Draw(RenderWindow& window);

	//void UpdateIdle(float dt);
	//void UpdateMove(float dt);

	void PlayIdle();
	void PlayMove();

	bool EqualFloat(float a, float b);
};

