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

SceneDev2::SceneDev2()
	:Scene(Scenes::Dev2), timer(0.f), attackTimer(1.f)
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
	slime->Init();

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
	FRAMEWORK->GetWindow().setMouseCursorGrabbed(false);
}

void SceneDev2::Exit()
{
	
}

void SceneDev2::Update(float dt)
{
	timer += dt;
	if ( timer > attackTimer && Keyboard::isKeyPressed(Keyboard::Key::Space) )
	{
		player->PlayAttack();
		timer = 0.f;
	}

	if ( Keyboard::isKeyPressed(Keyboard::Key::A) )
	{
		slime->PlayIdle();
	}
	if ( Keyboard::isKeyPressed(Keyboard::Key::S) )
	{
		slime->PlayMove();
	}
	player->Update(dt);
	slime->Update(dt);

	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	player->Draw(window);
	slime->Draw(window);
	Scene::Draw(window);
}
