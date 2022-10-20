#pragma once
#include "../GameObject/Object.h"
#include <list>

class Scene;

class UIMgr :public Object
{
protected:
	list<Object*> uiObjList;
	
	Scene* parentScene;

public:
	UIMgr(Scene* scene);
	virtual ~UIMgr();

	virtual void Init();
	virtual void Release();

	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

