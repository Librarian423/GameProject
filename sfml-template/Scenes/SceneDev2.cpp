#include "SceneDev2.h"
#include "SceneMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/SoundMgr.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"

SceneDev2::SceneDev2()
	:Scene(Scenes::Dev2)
{
}

SceneDev2::~SceneDev2()
{
}

void SceneDev2::Init()
{
	player = new Player();
	player->Init();
	
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
	if ( Keyboard::isKeyPressed(Keyboard::Key::Space) )
	{
		player->PlayAttack();
	}
	player->Update(dt);
	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	player->Draw(window);
	Scene::Draw(window);
}
