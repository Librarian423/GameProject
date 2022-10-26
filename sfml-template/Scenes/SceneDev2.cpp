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
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

SceneDev2::SceneDev2()
	:Scene(Scenes::Dev2), timer(0.f), attackTimer(1.f), slimeTimer(0.8f), slimeState(0)
{
}

SceneDev2::~SceneDev2()
{
}

void SceneDev2::Init()
{
	player = new Player();
	player->Init();
	
	slime = new Slime();
	slime->Init(player);
	slime->SetPos({ 500.f,500.f });
	Map1 = new CAP::SFMLMap("tilemap/", "map1.tmx");
	Map = new CAP::SFMLMap("tilemap/", "map2.tmx");

	for ( auto obj : objList )
	{
		obj->Init();
	}
}

void SceneDev2::Release()
{
}

void SceneDev2::Enter()
{
	Vector2i size = FRAMEWORK->GetWindowSize();
	worldView.setSize(size.x, size.y);
	worldView.setCenter(0.f, 0.f);
	uiView.setSize(size.x, size.y);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
	FRAMEWORK->GetWindow().setMouseCursorGrabbed(false);
}

void SceneDev2::Exit()
{
	
}

void SceneDev2::Update(float dt)
{
	slimeTimer -= dt;

	if ( (slime->GetState() != Slime::States::Dead) && slimeTimer < 0.f )
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

	slime->Update(dt);
	worldView.setCenter(player->GetPos());
	worldView.setCenter(player->GetPos());

	float border = 32.f;
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
	}

	player->Update(dt);
	
	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	Map1->draw(window, states);
	Map->draw(window, states);
	slime->Draw(window);
	player->Draw(window);
	
	window.setView(worldView);
	Scene::Draw(window);
}
