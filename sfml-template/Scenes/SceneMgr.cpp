#include "SceneMgr.h"
#include "SceneDev2.h"

bool SceneMgr::Init()
{
    //sceneMap[Scenes::Dev1] = new SceneDev1();
    sceneMap[Scenes::Dev2] = new SceneDev2();

    curScene = Scenes::Dev2;

    for ( auto pair : sceneMap )
    {
        pair.second->Init();
    }

    sceneMap[curScene]->Enter();

    return true;
}

Scene* SceneMgr::GetCurScene()
{
    return sceneMap[curScene];
}

Scene* SceneMgr::GetScene(Scenes scene)
{
    return sceneMap[scene];
}

void SceneMgr::ChangeScene(Scenes scene)
{
    sceneMap[curScene]->Exit();
    curScene = scene;
    sceneMap[curScene]->Enter();
}

void SceneMgr::Update(float dt)
{
    sceneMap[curScene]->Update(dt);
}

void SceneMgr::Draw(RenderWindow& window)
{
    sceneMap[curScene]->Draw(window);
}
