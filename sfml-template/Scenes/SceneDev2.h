#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>

class Player;
class Slime;
class Item;
class VertexArrayObj;

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
	VertexArrayObj* background;
	map<string, Vector2i> tileType;
	list<string> tileList;
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

	void CreateBackground(int width, int height, float quadWidth, float quadHeight);
	void SetTileNum();
	void ReadMap();
	Vector2i GetTile(string num);
};

