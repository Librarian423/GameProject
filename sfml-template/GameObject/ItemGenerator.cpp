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

void ItemGenerator::Generate(Vector2f pos)
{
	
	Scene* scene = SCENE_MGR->GetCurScene();
	//��� Ȯ��
	int healthChance = Utils::RandomRange(0, 10);
	Item::Types itemType;
	if ( healthChance <= 5 )
	{
		itemType = Item::Types::Coin;
	}
	else
	{
		itemType = Item::Types::Potion;
	}

	Item* item = new Item();
	item->Init(itemType);
	item->SetType(itemType);
	item->SetPlayer((Player*)scene->FindGameObj("Player"));
	switch ( itemType )
	{
	case Item::Types::Coin:
		item->SetValue(1);
		break;
	case Item::Types::Potion:
		item->SetValue(1);
		break;
	}
	item->SetPos(pos);
	itemList.push_back(item);
	scene->AddGameObj(item);
}
