#include "ItemGenerator.h"
#include "../Scenes/SceneMgr.h"
#include "Item.h"

ItemGenerator::ItemGenerator()
{
}

ItemGenerator::~ItemGenerator()
{
	Release();
}

void ItemGenerator::Init()
{

	itemList.push_back(new Item());
}

void ItemGenerator::Release()
{
	for ( auto& item: itemList )
	{
		delete item;
	}
	itemList.clear();
}

void ItemGenerator::Erase(int num)
{
	for ( auto it = itemList.begin(); it != itemList.end(); )
	{
		if ( (*it)->GetObjId() == num )
		{
			(*it)->SetActive(false);
			return;
		}
		else
		{
			it++;
		}
	}
}

void ItemGenerator::Update(float dt)
{
	for ( auto it = itemList.begin(); it != itemList.end(); )
	{
		if ( (*it)->GetActive() )
		{
			(*it)->Update(dt);
			it++;
		}
		else
		{
			(*it)->SetActive(false);
			it = itemList.erase(it);
		}
	}
}

void ItemGenerator::Draw(RenderWindow& window)
{
	for ( auto& item : itemList )
	{
		if ( item->GetActive() )
		{
			item->Draw(window);
		}
	}
}

void ItemGenerator::Generate(Vector2f pos, int value)
{
	Scene* scene = SCENE_MGR->GetCurScene();
	//µå¶ø È®·ü
	int healthChance = Utils::RandomRange(0, 100);
	Item::Types itemType;
	if ( healthChance <= 9 )
	{
		itemType = Item::Types::Coin;
	}
	else
	{
		itemType = Item::Types::Potion;
	}

	Item* item = new Item();
	item->Init();
	item->SetType(itemType);
	//item->SetPlayer((Player*)scene->FindGameObj("Player"));
	item->Init();

	Vector2f center = { 0,0 };
	bool success = false;
	int count = 0;

	while ( !success && count < 100 )
	{
		success = true;
		Vector2f position = pos;

		item->SetPos(position);

	}
	itemList.push_back(item);
}
