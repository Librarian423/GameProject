#include "SceneDev2.h"
#include "SceneMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/SoundMgr.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Slime.h"
#include "../GameObject/Boss.h"
#include "../GameObject/ItemBox.h"
#include "../GameObject/Item.h"
#include "../GameObject/ItemGenerator.h"
#include "../GameObject/VertexArrayObj.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;

SceneDev2::SceneDev2()
	:Scene(Scenes::Dev2), timer(0.f), attackTimer(1.f), boxCount(1)
{
}

SceneDev2::~SceneDev2()
{
}

void SceneDev2::Init()
{
	Release();
	ReadMap();
	SetTileNum();
	CreateBackground(20, 40, 32.f, 32.f);
	background->Init();

	itemBox = new ItemBox();
	itemBox->SetName("ItemBox");
	itemBox->Init();
	itemBox->SetActive(false);
	itemBox->SetBoxFalse();
	objList.push_back(itemBox);

	player = new Player();
	player->SetName("Player");
	player->Init();
	player->SetPos({ 130,500 });
	player->SetBackground(background);
	objList.push_back(player);

	itemBox->SetPlayer(player);

	CreateSlime(5);

	boss = new Boss();
	boss->SetName("Boss");
	boss->Init(player);
	boss->SetPos({ 1020.f,100.f });
	boss->SetBackground(background);
	boss->SetState(Boss::States::Idle);
	objList.push_back(boss);

	ITEM_GEN->Init();

	for ( auto obj : objList )
	{
		obj->Init();
	}
}

void SceneDev2::Release()
{
	Scene::Release();
}

void SceneDev2::Enter()
{

	ITEM_GEN->Release();
	
	Vector2i size = FRAMEWORK->GetWindowSize();

	worldView.setSize(size.x, size.y);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size.x, size.y);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	FRAMEWORK->GetWindow().setMouseCursorGrabbed(false);
	
}

void SceneDev2::Exit()
{
	ITEM_GEN->Release();
}

void SceneDev2::Update(float dt)
{
	for ( auto it = objList.begin(); it != objList.end(); )
	{
		if ( !(*it)->GetActive() && !(*it)->GetName().compare("Slime") )
		{
			delete (*it);
			it = objList.erase(it);
		}
		else
			it++;
	}
	
	worldView.setCenter(player->GetPos());
	worldView.setCenter(player->GetPos());
	
	//Gen itemBox
	if ( boxCount > 0 && boss->GetState() == Boss::States::Dead )
	{
		itemBox->SetBoxFalse(true);
		itemBox->SetState(ItemBox::States::Idle);
		boxCount--;
	}
	//Game End
	if ( player->GetCurrState() == Player::States::Dead )
	{
		
	}

	if ( InputMgr::GetKeyDown(Keyboard::Num1) )
	{
		itemBox->SetBoxFalse(true);
		itemBox->SetState(ItemBox::States::Idle);
		ITEM_GEN->Generate({ 300.f,300.f },true);
	}

	if ( InputMgr::GetKeyDown(Keyboard::Num2) )
	{
		itemBox->SetActive(false);
		ITEM_GEN->EraseKey();
	}
	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);
}

void SceneDev2::CreateBackground(int width, int height, float quadWidth, float quadHeight)
{
	if ( background == nullptr )
	{
		background = new VertexArrayObj();
		background->SetTexture(GetTexture("graphics/tilemap_packed.png"));
		objList.push_back(background);
	}
	
	Vector2f hitBoxPos = { 10.f,0.f };

	Vector2f startPos = background->GetPos();
	VertexArray& va = background->GetVA();
	va.clear();
	va.setPrimitiveType(Quads);
	va.resize(width * height * 4);
	Vector2f currPos = startPos;

	Vector2f offsets[4] = {
		{0, 0},
		{quadWidth, 0},
		{quadWidth, quadHeight},
		{0, quadHeight},
	};
	
	list<string>::iterator it;
	
	for ( int i = 0; i < width; ++i )
	{
		for ( int j = 0; j < height; ++j )
		{
			string tileNum = tileList.front();
			tileList.pop_front();
			Vector2i tile = tileType.find(tileNum)->second;
			int quadIndex = i * height + j;

			for ( int k = 0; k < 4; ++k )
			{
				int vertexIndex = quadIndex * 4 + k;
				va[vertexIndex].position = currPos + offsets[k];
				va[vertexIndex].texCoords = offsets[k];
				va[vertexIndex].texCoords.x += quadWidth * tile.x;
				va[vertexIndex].texCoords.y += quadHeight * tile.y;
				
			}
			it = find(wallNum.begin(), wallNum.end(), tileNum);
			if ( it != wallNum.end() )
			{
				background->MakeWallHitBox(hitBoxPos);
			}
			hitBoxPos.x += 32.f;
			currPos.x += (int)quadWidth;
		}
		hitBoxPos.x = 10.f;
		hitBoxPos.y += 32.f;
		currPos.x = startPos.x;
		currPos.y += (int)quadHeight;
		
	}
}

void SceneDev2::SetTileNum()
{
	int count = 1;
	for ( int i = 0; i < 5; i++ )
	{
		for ( int j = 0; j < 12; j++ )
		{
			tileType.insert({ to_string(count), {j,i} });
			count++;
		}
	}
}

void SceneDev2::ReadMap()
{
	ifstream file("tilemap/map.txt");
	if ( !file )
	{
		cout << "error";
		return;
	}
	cout << "file open" << endl;

	string temp;
	string num;
	int line = 0;
	while ( getline(file, temp) )
	{
		num.clear();
		line++;
		num = temp[0];
		for ( int i = 1; i < temp.length(); i++ )
		{
			if ( temp[i] == ',' )
			{
				tileList.push_back(num);
				num.clear();
				continue;
			}
			
			else
			{
				num += temp[i];
			}
		}
	}
	file.close();
	cout << "file close" << endl;
}

void SceneDev2::CreateSlime(int num)
{
	for ( int i = 0; i < num; i++ )
	{
		Slime* slime = new Slime();
		slime->SetName("Slime");
		slime->Init(player);
		slime->SetPos({ Utils::RandomRange(1000.f,1100.f),Utils::RandomRange(200.f,500.f) });
		slime->SetBackground(background);
		slime->SetState(Slime::States::Idle);
		objList.push_back(slime);
	}
}