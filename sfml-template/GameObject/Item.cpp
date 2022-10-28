#include "Item.h"
#include "HitBox.h"
#include "Player.h"
#include "ItemGenerator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"

#include <iostream>

Item::Item()
    :currState(Types::None), isHitBox(true), value(0)
{
}

Item::~Item()
{
}

void Item::Init(Types type)
{
    currState = type;

    switch ( type )
    {
    case Item::Types::None:
        break;
    case Item::Types::Coin:
        animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Coin"));
        sprite.setScale({ 1.2f,1.2f });
        animator.SetTarget(&sprite);
        break;
    case Item::Types::Potion:
        animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Potion"));
        sprite.setScale({ 3.f,3.f });
        animator.SetTarget(&sprite);
        break;
    case Item::Types::Key:
        break;
    }
    
    //SetType(Types::Coin);
    itemHitbox = new HitBox();
    itemHitbox->SetHitbox({ 0,0,20.f,20.f });
   
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
    itemHitbox->SetPos({ GetPos().x,GetPos().y + 10 });
    animator.Update(dt);
    //player Á¢ÃË
    if ( Utils::OBB(itemHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
    {
        player->OnPickupItem(this);
        SetActive(false);
        ITEM_GEN->Erase(this->GetObjId());
    }
    if ( InputMgr::GetKeyDown(Keyboard::F1) )
    {
        isHitBox = !isHitBox;
    }
    SpriteObj::Update(dt);
}

void Item::Draw(RenderWindow& window)
{
    SpriteObj::Draw(window);
    if ( isHitBox )
    {
        itemHitbox->Draw(window);
    }
}

void Item::SetPlayer(Player* p)
{
    player = p;
}