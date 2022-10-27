#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include "../TileMapLoader.h"

class Player;
class Slime;
class Item;
class SceneDev2 :public Scene
{

protected:
	Player* player;
	Slime* slime;

	int slimeState;

	float timer;
	float attackTimer;
	float slimeTimer;

	//map
	CAP::SFMLMap* Map1;
	CAP::SFMLMap* Map;
	RenderStates states;

public:
	SceneDev2();
	virtual ~SceneDev2();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

