#pragma once
#include "../3rd/Singleton.h"
#include "Object.h"
#include <list>

class Item;

class ItemGenerator :public Singleton<ItemGenerator>
{
protected:
	list<Item*>itemList;
	Vector2f pos;
	
public:
	ItemGenerator();
	virtual ~ItemGenerator();

	void Init();
	void Release();
	void Erase(int num);
	void Update(float dt);
	void Draw(RenderWindow& window);
	void Generate(Vector2f pos);
};
#define ITEM_GEN (ItemGenerator::GetInstance())