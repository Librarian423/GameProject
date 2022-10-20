#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"

class Player;
class Slime;

class SceneDev2 :public Scene
{

protected:
	Player* player;
	Slime* slime;
	
	float timer;
	float attackTimer;


public:
	SceneDev2();
	virtual ~SceneDev2();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

