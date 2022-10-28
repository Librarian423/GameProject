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
#include "../GameObject/Item.h"
#include "../GameObject/ItemGenerator.h"
#include "../GameObject/VertexArrayObj.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;

SceneDev2::SceneDev2()
	:Scene(Scenes::Dev2), timer(0.f), attackTimer(1.f), slimeTimer(0.8f), slimeState(0)
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
	background->SetPos({ 0,0 });
	background->SetOrigin(Origins::MC);

	player = new Player();
	player->SetName("Player");
	player->Init();
	objList.push_back(player);
	
	slime = new Slime();
	slime->SetName("Slime");
	slime->Init(player);
	slime->SetPos({ 500.f,500.f });
	objList.push_back(slime);
	
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
	slimeTimer -= dt;
	if ( slime->GetState() != Slime::States::Dead )
	{
		if ( slimeTimer < 0.f )
		{
			cout << "timer" << endl;
			if ( slimeState % 2 == 0 )
			{
				slime->SetState(Slime::States::Idle);
				slimeState = 1;
			}
			else
			{
				slime->SetState(Slime::States::Move);
				slimeState = 0;
			}
			slimeTimer = 5.f;
		}
	}
	else if ( slimeTimer < 0.f && slime->GetState() == Slime::States::Dead )
	{
		slime->Init(player);
		slime->SetState(Slime::States::Idle);
		slime->SetPos({ 200.f,200.f });
	}
	
	worldView.setCenter(player->GetPos());
	worldView.setCenter(player->GetPos());

	/*float border = 32.f;
	FloatRect wallBound = Map->GetGlobalBounds();
	Vector2f pos;
	pos.x = Utils::Clamp(player->GetPos().x,
		wallBound.left + border,
		wallBound.left + wallBound.width - border);
	pos.y = Utils::Clamp(player->GetPos().y,
		wallBound.top + border,
		wallBound.top + wallBound.height - border);

	if ( pos != player->GetPos() )
	{
		player->SetPos(pos);
	}*/

	if ( InputMgr::GetKeyDown(Keyboard::Num1) )
	{
		ITEM_GEN->Generate({ 100.f,100.f });
	}

	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	//Map1->draw(window, states);
	//Map->draw(window, states);
	//slime->Draw(window);
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
			currPos.x += (int)quadWidth;
		}
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

Vector2i SceneDev2::GetTile(string num)
{
	for ( auto it = tileType.begin(); it != tileType.end(); it++ )
	{

	}
	return Vector2i();
}
