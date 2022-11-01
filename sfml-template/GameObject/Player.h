#pragma once
#include "SpriteObj.h"
#include "Slime.h"
#include "../Animation/Animator.h"
#include <list>

using namespace std;

class HitBox;
class Item;
class VertexArrayObj;

class Player : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Attack,
		Dead,
	};
protected:
	Animator animator;

	RectangleShape healthBar;

	HitBox *playerHitbox;
	HitBox *attackHitbox;

	States currState;
	
	float speed;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f prevPosition;

	VertexArrayObj* background;
	
	float timer;
	float attackTime;

	int damage;
	int hp;
	int maxHp;

	bool isHitBox;
	bool isKey;
	bool isAlive;
public:
	Player();
	virtual~Player();
	
	void Init();

	void SetState(States newState);
	void SetBackground(VertexArrayObj* bk);

	void Update(float dt);
	void Draw(RenderWindow& window);

	void PlayAttack();

	void Dash(float dt);
	void OnCompleteAttack();
	void OnCompleteDead();

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	
	bool EqualFloat(float a, float b);

	HitBox* GetPlayerHitBox();
	HitBox* GetAttackHitbox();
	Vector2f GetPlayerDir() { return direction; }
	Vector2f GetPlayerLastDir() { return lastDirection; }
	int GetDamage() { return damage; }
	States GetCurrState() { return currState; }

	void SetHp(int num);
	void SetHpBar();
	void OnPickupItem(Item* item);
	void SetPlayerPos();
	void SetIsKey() { isKey = false; }
	bool GetIsKey() { return isKey; }
};

