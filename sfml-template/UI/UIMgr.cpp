#include "UIMgr.h"

UIMgr::UIMgr(Scene* scene)
    :parentScene(scene)
{
}

UIMgr::~UIMgr()
{
}

void UIMgr::Init()
{
    Object::Init();
    for ( auto uiObj : uiObjList )
    {
        uiObj->Init();
    }
}

void UIMgr::Release()
{
    Object::Release();
    for ( auto uiObj : uiObjList )
    {
        uiObj->Release();
    }
}

void UIMgr::Reset()
{
    Object::Reset();
    for ( auto uiObj : uiObjList )
    {
        uiObj->Reset();
    }
}

void UIMgr::SetPos(const Vector2f& pos)
{
    Vector2f delta = pos - position;
    for ( auto uiObj : uiObjList )
    {
        uiObj->Translate(delta);
    }
    position = pos;
}

void UIMgr::Update(float dt)
{
    if ( !enabled )
    {
        return;
    }

    for ( auto uiObj : uiObjList )
    {
        if ( uiObj->GetActive() )
        {
            uiObj->Update(dt);
        }
    }
}

void UIMgr::Draw(RenderWindow& window)
{
    if ( !enabled )
    {
        return;
    }
    for ( auto uiObj : uiObjList )
    {
        if ( uiObj->GetActive() )
        {
            uiObj->Draw(window);
        }
    }
}
