#include "Item.h"
#include "HitBox.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include <iostream>

Item::Item()
{
}

Item::~Item()
{
}

void Item::Init()
{
    sprite.setScale({ 3.f,3.f });
    animator.SetTarget(&sprite);

    animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Coin"));
    animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Potion"));
    
    //SetType(Types::Coin);
    itemHitbox = new HitBox();
    itemHitbox->SetHitbox({ 0,0,35.f,30.f });
    itemHitbox->SetPos(GetPos());
   
    SpriteObj::Init();
}

void Item::SetType(Types newTypes)
{
    currState = newTypes;

    switch ( newTypes )
    {
    case Item::Types::Coin:
        animator.Play("Coin");
        break;
    case Item::Types::Potion:
        animator.Play("Potion");
        break;
    case Item::Types::Key:
        break;
    }

}

void Item::Update(float dt)
{
    animator.Update(dt);
   
    SpriteObj::Update(dt);
}

void Item::Draw(RenderWindow& window)
{
    SpriteObj::Draw(window);
}

void Item::SetPlayer(Player* p)
{
    player = p;
}

bool Item::EqualFloat(float a, float b)
{
    return false;
}
