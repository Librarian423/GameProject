#pragma once
#include "Scene.h"
#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class SceneMgr :public Singleton<SceneMgr>
{
private:
	map<Scenes, Scene*> sceneMap;
	Scenes curScene;

public:
	bool Init();

	Scene* GetCurScene();
	Scene* GetScene(Scenes scene);

	void ChangeScene(Scenes scene);

	void Update(float dt);
	void Draw(RenderWindow& window);
};

#define SCENE_MGR (SceneMgr::GetInstance())
